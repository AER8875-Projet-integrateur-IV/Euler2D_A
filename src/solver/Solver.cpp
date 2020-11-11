#include "Solver.hpp"
#include <cmath>
#include <stdlib.h>//pour la valeur absolue
#include "../utils/logger/Logger.hpp"
#include <string>
#include <iostream>

Solver::Solver(Mesh* mesh, ees2d::io::InputParser* IC)
{
	Logger::getInstance()->AddLog("________________________Solver initialisation________________________",1);
    m_mesh = mesh;
	m_inputParameters = IC;

	Logger::getInstance()->AddLog("solving initial conditions...",1);

	m_Winf = new W();

	double E = IC->m_Pressure / IC->m_Density / IC->m_Gamma + 0.5 * pow(IC->m_Mach, 2);//POURQUOI MACH ICI?
	m_Winf->E = E;
	m_Winf->H = E + IC->m_Pressure / IC->m_Density;
	m_Winf->P = IC->m_Pressure;
	m_Winf->rho = IC->m_Density;
	m_Winf->u = cos(IC->m_aoa*M_PI/180)*IC->m_Mach;
	m_Winf->v = sin(IC->m_aoa*M_PI/180)*IC->m_Mach;

	// initialise element2W
	m_element2W = new W[m_mesh->m_nElementTot];
	for(int iElement = 0;iElement<m_mesh->m_nElementTot;iElement++){
		m_element2W[iElement] = *m_Winf;
	}

	// initialise face2Fc
	m_face2Fc = new Fc[m_mesh->m_nFace];//Enlever? a voir


	// initialise markers
	Logger::getInstance()->AddLog("Defining the type of the border conditions face elements...",1);
	m_mesh->m_markers->Check4Face(m_mesh->m_face2Node, m_mesh->m_nFace);
	Logger::getInstance()->AddLog("Defining the type of the border conditions ghost cells...",1);
	m_mesh->m_markers->FindElements(m_mesh);

	// Chose Scheme
	std::string schemeName = IC->m_scheme;
	if(schemeName=="AVERAGE"){
		m_scheme = &Solver::ConvectiveFluxAverageScheme;
	} else if(schemeName=="ROE"){
		m_scheme = &Solver::ConvectiveFluxRoeScheme;
	} else
	{
		throw std::logic_error(schemeName + " scheme not implemented");
	}


	Logger::getInstance()->AddLog("________________________Ghost cells initialisation____________________", 1);
	// Ajouter le code qui permet de update les Ghost cells


	Logger::getInstance()->AddLog("________________________Iterative Process Start_______________________", 1);
	// prend un while
	// WHILE

	// On doit d'abord mettre les residus a 0 pour tous les elements!! au debut de chaque iteration
	// TODO set residuals to 0 for all elements

	// Boucle sur les faces pour calculer les Flux
	for (int iFace = 0; iFace < m_mesh->m_nFace; iFace++) {
		// Rearrangement du vecteur normal a la face si necessaire
		Solver::DotProduct(iFace);

		// Calcul du flux convectix de la face iFace
		(this->*m_scheme)(iFace);

		// Mise a jour des Residus des elements connectes a iFace
		Solver::CalculateResiduals(iFace);
	}

	for (int iElem = 0; iElem < m_mesh->m_nElement; iElem++) {
		// Calcul du pas de temps pour chaque element
		double LocalTimeStep = Solver::LocalTimeStep(iElem);

		Solver::CalculateDeltaW(iElem, LocalTimeStep);

		Solver::UpdateW(iElem);
	}

	//Reset les conditions limites
	// iteration += 1;
	// Calcul dur RMS des Residus
	// Reset des residus (ici ou au debut de la boucle)
}

Solver::~Solver()
{
	delete[] m_element2W;
	delete[] m_face2Fc;
	delete m_Winf;
}



void Solver::SolveFc(){
	// while (residuals>minresiduals && iteration<MaxIteration)

	//Update --> uopdate les conditions limites


	for (int iFace = 0; iFace < m_mesh->m_nFace; iFace++) {
		// Calcul Fc
		(this->*m_scheme)(iFace);
	}

	// m_mesh->m_markers.update()
}

void Solver::ConvectiveFluxAverageScheme(int iFace) {
	// Schema avec les moyennes
	int elem1 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 0];
	int elem2 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 1];

	double rho_avg = 0.5 * (this->m_element2W[elem1].rho + this->m_element2W[elem2].rho);
	double u_avg = 0.5 * (this->m_element2W[elem1].u + this->m_element2W[elem2].u);
	double v_avg = 0.5 * (this->m_element2W[elem1].v + this->m_element2W[elem2].v);
	double P_avg = 0.5 * (this->m_element2W[elem1].P + this->m_element2W[elem2].P);
	double H_avg = 0.5 * (this->m_element2W[elem1].H + this->m_element2W[elem2].H);

	// Calcul de la vitesse contravariante
	double V = u_avg * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + v_avg * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1];//TODO changer le 2 pour le cas 3D et mettre le nombre de noeuds de la face

	// Calcul de Fc
	this->m_face2Fc[iFace].rho = rho_avg * V;
	this->m_face2Fc[iFace].u = rho_avg * u_avg * V + m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] * P_avg;
	this->m_face2Fc[iFace].v = rho_avg * v_avg * V + m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1] * P_avg;
	this->m_face2Fc[iFace].H = rho_avg * H_avg * V;
}

void Solver::ConvectiveFluxRoeScheme(int iFace) {
	// Schema de Roe

	int elem1 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 0];
	int elem2 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 1];

	// Pour faciliter la comprehension, calcul de sqrt(rho_L) et sqrt(rho_R)
	double sqrtRhoL = pow(this->m_element2W[elem1].rho, 0.5);
	double sqrtRhoR = pow(this->m_element2W[elem2].rho, 0.5);

	// 1. Calcul des Roe-averaged variables
	double rho_tilde = sqrtRhoL * sqrtRhoR;
	double u_tilde = (this->m_element2W[elem1].u * sqrtRhoL + this->m_element2W[elem2].u * sqrtRhoR) / (sqrtRhoL + sqrtRhoR);
	double v_tilde = (this->m_element2W[elem1].v * sqrtRhoL + this->m_element2W[elem2].v * sqrtRhoR) / (sqrtRhoL + sqrtRhoR);
	double H_tilde = (this->m_element2W[elem1].H * sqrtRhoL + this->m_element2W[elem2].H * sqrtRhoR) / (sqrtRhoL + sqrtRhoR);

	double q_tilde2 = pow(u_tilde, 2) + pow(v_tilde, 2);
	double c_tilde = pow((m_inputParameters->m_Gamma - 1) * (H_tilde - q_tilde2 / 2), 0.5);
	double V_tilde = u_tilde * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + v_tilde * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1];

	// 2. Calcul des Delta
	double rhoDelta = this->m_element2W[elem2].rho - this->m_element2W[elem1].rho;
	double uDelta = this->m_element2W[elem2].u - this->m_element2W[elem1].u;
	double vDelta = this->m_element2W[elem2].v - this->m_element2W[elem1].v;
	double pDelta = this->m_element2W[elem2].P - this->m_element2W[elem1].P;
	double VDelta = (this->m_element2W[elem2].u * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + this->m_element2W[elem2].v * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1]) -
	                (this->m_element2W[elem1].u * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + this->m_element2W[elem1].v * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1]);

	// 3. Calcul de Fc pour l'element de gauche (FcL), elem1
	double V_FcL = this->m_element2W[elem1].u * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + this->m_element2W[elem1].v * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1];

	double rhoV_FcL = this->m_element2W[elem1].rho * V_FcL;
	double rhouV_FcL = this->m_element2W[elem1].rho * this->m_element2W[elem1].u * V_FcL + m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] * this->m_element2W[elem1].P;
	double rhovV_FcL = this->m_element2W[elem1].rho * this->m_element2W[elem1].v * V_FcL + m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1] * this->m_element2W[elem1].P;
	double rhoHV_FcL = this->m_element2W[elem1].rho * this->m_element2W[elem1].H * V_FcL;


	// 4. Calcul de Fc pour l'element de droite (FcR), elem2
	double V_FcR = this->m_element2W[elem2].u * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + this->m_element2W[elem2].v * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1];

	double rhoV_FcR = this->m_element2W[elem2].rho * V_FcL;
	double rhouV_FcR = this->m_element2W[elem2].rho * this->m_element2W[elem2].u * V_FcL + m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] * this->m_element2W[elem2].P;
	double rhovV_FcR = this->m_element2W[elem2].rho * this->m_element2W[elem2].v * V_FcL + m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1] * this->m_element2W[elem2].P;
	double rhoHV_FcR = this->m_element2W[elem2].rho * this->m_element2W[elem2].H * V_FcL;


	// 5. Calcul de la matrice de Roe
	// 5.01 Calul du facteur de correction de Hartens, delta
	double a_elem1 = pow(m_inputParameters->m_Gamma * this->m_element2W[elem1].P / this->m_element2W[elem1].rho, 0.5);//Vitesse du son de l'element 1
	double a_elem2 = pow(m_inputParameters->m_Gamma * this->m_element2W[elem2].P / this->m_element2W[elem2].rho, 0.5);//Vitesse du son de l'element 2
	double a_avg = (a_elem1 + a_elem2) / 2;                                                                           //Moyenne de la vitesse du son

	double delta = 0.1 * a_avg;  //Premier cas possible
	//double delta = 0.1 * c_tilde;// Deuxieme cas possible, a voir lequel prendre

	// Pour DeltaFc1
	double HartensCorrectionF1;
	if (std::abs(V_tilde - c_tilde) > delta) {
		HartensCorrectionF1 = std::abs(V_tilde - c_tilde);
	} else {
		HartensCorrectionF1 = (pow(V_tilde - c_tilde, 2) + pow(delta, 2)) / (2 * delta);
	}

	// Pour DeltaFc5
	double HartensCorrectionF5;
	if (std::abs(V_tilde + c_tilde) > delta) {
		HartensCorrectionF5 = std::abs(V_tilde + c_tilde);
	} else {
		HartensCorrectionF5 = (pow(V_tilde + c_tilde, 2) + pow(delta, 2)) / (2 * delta);
	}

	// 5.1 Calcul de DeltaFc1
	double termFc1 = HartensCorrectionF1 * ((pDelta - rho_tilde * c_tilde * VDelta) / (2 * pow(c_tilde, 2)));

	double rhoV_Fc1 = termFc1;
	double rhouV_Fc1 = termFc1 * (u_tilde - c_tilde * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0]);
	double rhovV_Fc1 = termFc1 * (v_tilde - c_tilde * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1]);
	double rhoHV_Fc1 = termFc1 * (H_tilde - c_tilde * V_tilde);

	// 5.2 Calcul de DeltaFc234
	double termFc234 = std::abs(V_tilde) * (rhoDelta - pDelta / pow(c_tilde, 2));

	double rhoV_Fc234 = termFc234;
	double rhouV_Fc234 = termFc234 * u_tilde + rho_tilde * (uDelta - VDelta * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0]);
	double rhovV_Fc234 = termFc234 * v_tilde + rho_tilde * (vDelta - VDelta * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1]);
	double rhoHV_Fc234 = termFc234 * q_tilde2 / 2 + rho_tilde * (u_tilde * uDelta + v_tilde * vDelta - V_tilde * VDelta);

	// 5.2 Calcul de DeltaFc5
	double termFc5 = HartensCorrectionF5 * ((pDelta - rho_tilde * c_tilde * VDelta) / (2 * pow(c_tilde, 2)));

	double rhoV_Fc5 = termFc5;
	double rhouV_Fc5 = termFc5 * (u_tilde + c_tilde * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0]);
	double rhovV_Fc5 = termFc5 * (v_tilde + c_tilde * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1]);
	double rhoHV_Fc5 = termFc5 * (H_tilde + c_tilde * V_tilde);

	// 6. Calcul de Fc
	double rhoV_Fc = 0.5 * (rhoV_FcL + rhoV_FcR - rhoV_Fc1 - rhoV_Fc234 - rhoV_Fc5);
	double rhouV_Fc = 0.5 * (rhouV_FcL + rhouV_FcR - rhouV_Fc1 - rhouV_Fc234 - rhouV_Fc5);
	double rhovV_Fc = 0.5 * (rhovV_FcL + rhovV_FcR - rhovV_Fc1 - rhovV_Fc234 - rhovV_Fc5);
	double rhoHV_Fc = 0.5 * (rhoHV_FcL + rhoHV_FcR - rhoHV_Fc1 - rhoHV_Fc234 - rhoHV_Fc5);

	// 7. Met dans le pointeur
	this->m_face2Fc[iFace].rho = rhoV_Fc;
	this->m_face2Fc[iFace].u = rhouV_Fc;
	this->m_face2Fc[iFace].v = rhovV_Fc;
	this->m_face2Fc[iFace].H = rhoHV_Fc;
}

void Solver::DotProduct(int iFace) {
	int elem1 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 0];
	int elem2 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 1];

	//Calcul du produit scalaire pour savoir s'il faut changer le sens de la normale a la face
	double xElem1 = m_mesh->m_element2Center[m_mesh->m_nDime * elem1 + 0];
	double yElem1 = m_mesh->m_element2Center[m_mesh->m_nDime * elem1 + 1];

	double xElem2 = m_mesh->m_element2Center[m_mesh->m_nDime * elem2 + 0];
	double yElem2 = m_mesh->m_element2Center[m_mesh->m_nDime * elem2 + 1];

	double xVecteur = xElem2 - xElem1;
	double yVecteur = yElem2 - yElem1;

	double dotProduct = xVecteur * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] + yVecteur * m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1];

	if (dotProduct > 0) {//TODO a verifier pour cette condition
		// Si on veut que la normale pointe vers l'element 1 (on va ensuite additionner le flux pour l'element 1 et soustraire le flux a l'element 2)
		m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 0] *= -1;
		m_mesh->m_face2Normal[m_mesh->m_nDime * iFace + 1] *= -1;
	}
}

double Solver::LocalTimeStep(int iElem) {
	double CFL = 1;//CFL = m_inputParameters->CFL

	// 1. Calcul des lambdas

	// 1.1 Calcul des Sum_Sx et Sum_Sy
	double Sum_Sx = 0;
	double Sum_Sy = 0;

	//On boucle sur les faces de l'element iElem
	int startI = m_mesh->m_element2FaceStart[iElem];
	int endI = m_mesh->m_element2FaceStart[iElem + 1];

	int iFace;
	for (int i = startI; i < endI; ++i) {
		iFace = m_mesh->m_element2Face[i];
		Sum_Sx += std::abs(m_mesh->m_face2FaceVector[m_mesh->m_nDime * iFace + 0]);
		Sum_Sy += std::abs(m_mesh->m_face2FaceVector[m_mesh->m_nDime * iFace + 1]);
	}

	// 1.2 Calcul des lambdas
	double lambda_Cx, lambda_Cy;
	double c = pow(m_inputParameters->m_Gamma * this->m_element2W[iElem].P / this->m_element2W[iElem].rho, 0.5);
	lambda_Cx = 0.5 * (std::abs(this->m_element2W[iElem].u) + c) * Sum_Sx;
	lambda_Cy = 0.5 * (std::abs(this->m_element2W[iElem].v) + c) * Sum_Sy;

	// 2. Calcul du TimeStep
	double LocalTimeStep = CFL * m_mesh->m_element2Volume[iElem] / (lambda_Cx + lambda_Cy);

	return LocalTimeStep;
}

void Solver::CalculateResiduals(int iFace) {
	int elem1 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 0];
	int elem2 = m_mesh->m_face2Element[m_mesh->m_nDime * iFace + 1];
	double Area = m_mesh->m_face2Area[iFace];

	this->m_element2Residuals[elem1].rho += this->m_face2Fc[iFace].rho * Area;
	this->m_element2Residuals[elem1].u += this->m_face2Fc[iFace].u * Area;
	this->m_element2Residuals[elem1].v += this->m_face2Fc[iFace].v * Area;
	this->m_element2Residuals[elem1].E += this->m_face2Fc[iFace].H * Area;

	this->m_element2Residuals[elem2].rho -= this->m_face2Fc[iFace].rho * Area;
	this->m_element2Residuals[elem2].u -= this->m_face2Fc[iFace].u * Area;
	this->m_element2Residuals[elem2].v -= this->m_face2Fc[iFace].v * Area;
	this->m_element2Residuals[elem2].E -= this->m_face2Fc[iFace].H * Area;
}

void Solver::CalculateDeltaW(int iElem, double LocalTimeStep) {
	this->m_element2DeltaW[iElem].rho = -LocalTimeStep * this->m_element2Residuals[iElem].rho / m_mesh->m_element2Volume[iElem];
	this->m_element2DeltaW[iElem].u = -LocalTimeStep * this->m_element2Residuals[iElem].u / m_mesh->m_element2Volume[iElem];
	this->m_element2DeltaW[iElem].v = -LocalTimeStep * this->m_element2Residuals[iElem].v / m_mesh->m_element2Volume[iElem];
	this->m_element2DeltaW[iElem].E = -LocalTimeStep * this->m_element2Residuals[iElem].E / m_mesh->m_element2Volume[iElem];
}

void Solver::UpdateW(int iElem) {
	this->m_element2W[iElem].rho += this->m_element2DeltaW[iElem].rho;
	this->m_element2W[iElem].u += this->m_element2DeltaW[iElem].u / this->m_element2W[iElem].rho;
	this->m_element2W[iElem].v += this->m_element2DeltaW[iElem].v / this->m_element2W[iElem].rho;
	this->m_element2W[iElem].E += this->m_element2DeltaW[iElem].E / this->m_element2W[iElem].rho;
	this->m_element2W[iElem].P += this->m_element2W[iElem].rho * m_inputParameters->m_Gamma * (this->m_element2W[iElem].E - 0.5 * (pow(this->m_element2W[iElem].u, 2) + pow(this->m_element2W[iElem].v, 2)));
	this->m_element2W[iElem].H += this->m_element2W[iElem].E + this->m_element2W[iElem].P / this->m_element2W[iElem].rho;
}
