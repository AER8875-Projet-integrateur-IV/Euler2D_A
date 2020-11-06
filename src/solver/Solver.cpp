#include "Solver.hpp"
#include <stdlib.h>//pour la valeur absolue

Solver::Solver(Mesh* mesh, InitialConditions* IC)
{
    m_mesh = mesh;

    // Initialisation
}

Solver::~Solver()
{
}


int elem1, elem2;
double rho_avg, u_avg, v_avg, H_avg, V;

void Solver::SolveFc(){
	// while (residuals>minresiduals && iteration<MaxIteration)


	// Calcul Fc interne
    for(int iFace = 0;iFace<m_mesh->m_nFaceNoBoundaries;iFace++){
        // Calcul Fc

		//this->ConvectiveFluxAverageScheme(iFace);

		this->ConvectiveFluxRoeScheme(iFace);
	}

	// m_mesh->m_markers.update()
}

void Solver::ConvectiveFluxAverageScheme(int iFace) {
	// Schema avec les moyennes
	int elem1 = m_mesh->m_face2Element[2 * iFace + 0];
	int elem2 = m_mesh->m_face2Element[2 * iFace + 1];

	double rho_avg = 0.5 * (this->m_element2W[elem1].rho + this->m_element2W[elem2].rho);
	double u_avg = 0.5 * (this->m_element2W[elem1].u + this->m_element2W[elem2].u);
	double v_avg = 0.5 * (this->m_element2W[elem1].v + this->m_element2W[elem2].v);
	double P_avg = 0.5 * (this->m_element2W[elem1].P + this->m_element2W[elem2].P);
	double H_avg = 0.5 * (this->m_element2W[elem1].H + this->m_element2W[elem2].H);

	// Calcul de la vitesse contravariante
	double V = u_avg * m_mesh->m_face2Normal[2 * iFace + 0] + v_avg * m_mesh->m_face2Normal[2 * iface + 1];//TODO changer le 2 pour le cas 3D et mettre le nombre de noeuds de la face

	// Calcul de Fc
	this->m_face2Fc[iFace].rho = rho_avg * V;
	this->m_face2Fc[iFace].u = rho_avg * u_avg * V + m_mesh->m_face2Normal[2 * iFace + 0] * p_avg;
	this->m_face2Fc[iFace].v = rho_avg * v_avg * V + m_mesh->m_face2Normal[2 * iFace + 1] * p_avg;
	this->m_face2Fc[iFace].H = rho_avg * H_avg * V;
}

void Solver::ConvectiveFluxRoeScheme(int iFace) {
	// Schema de Roe

	int elem1 = m_mesh->m_face2Element[2 * iFace + 0];
	int elem2 = m_mesh->m_face2Element[2 * iFace + 1];

	// Pour faciliter la comprehension, calcul de sqrt(rho_L) et sqrt(rho_R)
	double sqrtRhoL = pow(this->m_element2W[elem1].rho, 0.5);
	double sqrtRhoR = pow(this->m_element2W[elem2].rho, 0.5);

	// 1. Calcul des Roe-averaged variables
	double rho_tilde = sqrtRhoL * sqrtRhoR;
	double u_tilde = (this->m_element2W[elem1].u * sqrtRhoL + this->m_element2W[elem2].u * sqrtRhoR) / (sqrtRhoL + sqrtRhoR);
	double v_tilde = (this->m_element2W[elem1].v * sqrtRhoL + this->m_element2W[elem2].v * sqrtRhoR) / (sqrtRhoL + sqrtRhoR);
	double H_tilde = (this->m_element2W[elem1].H * sqrtRhoL + this->m_element2W[elem2].H * sqrtRhoR) / (sqrtRhoL + sqrtRhoR);

	double q_tilde2 = pow(u_tilde, 2) + pow(v_tile, 2);
	double c_tilde = pow((m_inputParameters->m_Gamma - 1) * (H_tilde - q_tilde2 / 2), 0.5);
	double V_tilde = u_tilde * m_mesh->m_face2Normal[2 * iFace + 0] + v_tilde * m_mesh->m_face2Normal[2 * iFace + 1];

	// 2. Calcul des Delta
	double rhoDelta = this->m_element2W[elem2].rho - this->m_element2W[elem1].rho;
	double uDelta = this->m_element2W[elem2].u - this->m_element2W[elem1].u;
	double vDelta = this->m_element2W[elem2].v - this->m_element2W[elem1].v;
	double pDelta = this->m_element2W[elem2].p - this->m_element2W[elem1].p;
	double VDelta = (this->m_element2W[elem2].u * m_mesh->m_face2Normal[2 * iFace + 0] + this->m_element2W[elem2].v * m_mesh->m_face2Normal[2 * iFace + 1]) -
	                (this->m_element2W[elem1].u * m_mesh->m_face2Normal[2 * iFace + 0] + this->m_element2W[elem1].v * m_mesh->m_face2Normal[2 * iFace + 1]);

	// 3. Calcul de Fc pour l'element de gauche (FcL), elem1
	double V_FcL = this->m_element2W[elem1].u * m_mesh->m_face2Normal[2 * iFace + 0] + this->m_element2W[elem1].v * m_mesh->m_face2Normal[2 * iFace + 1];

	double rhoV_FcL = this->m_element2W[elem1].rho * V_FcL;
	double rhouV_FcL = this->m_element2W[elem1].rho * this->m_element2W[elem1].u * V_Fcl + m_mesh->m_face2Normal[2 * iface + 0] * this->m_element2W[elem1].p;
	double rhovV_FcL = this->m_element2W[elem1].rho * this->m_element2W[elem1].v * V_Fcl + m_mesh->m_face2Normal[2 * iface + 1] * this->m_element2W[elem1].p;
	double rhoHV_FcL = this->m_element2W[elem1].rho * this->m_element2W[elem1].H * V_FcL;


	// 4. Calcul de Fc pour l'element de droite (FcR), elem2
	double V_FcR = this->m_element2W[elem2].u * m_mesh->m_face2Normal[2 * iFace + 0] + this->m_element2W[elem2].v * m_mesh->m_face2Normal[2 * iFace + 1];

	double rhoV_FcR = this->m_element2W[elem2].rho * V_FcL;
	double rhouV_FcR = this->m_element2W[elem2].rho * this->m_element2W[elem2].u * V_Fcl + m_mesh->m_face2Normal[2 * iface + 0] * this->m_element2W[elem2].p;
	double rhovV_FcR = this->m_element2W[elem2].rho * this->m_element2W[elem2].v * V_Fcl + m_mesh->m_face2Normal[2 * iface + 1] * this->m_element2W[elem2].p;
	double rhoHV_FcR = this->m_element2W[elem2].rho * this->m_element2W[elem2].H * V_FcL;


	// 5. Calcul de la matrice de Roe
	// 5.01 Calul du facteur de correction de Hartens, delta
	double a_elem1 = pow(m_inputParameters->m_Gamma * this->m_element2W[elem1].p / this->m_element2W[elem1].rho, 0.5);//Vitesse du son de l'element 1
	double a_elem2 = pow(m_inputParameters->m_Gamma * this->m_element2W[elem2].p / this->m_element2W[elem2].rho, 0.5);//Vitesse du son de l'element 2
	double a_avg = (a_elem1 + a_elem2) / 2;                                                                           //Moyenne de la vitesse du son

	double delta = 0.1 * a_avg;  //Premier cas possible
	double delta = 0.1 * c_tilde;// Deuxieme cas possible, a voir lequel prendre

	// Pour DeltaFc1
	if (abs(V_tilde - c_tilde) > delta) {
		double HartensCorrectionF1 = abs(V_tilde - c_tilde);
	} else {
		double HartensCorrectionF1 = (pow(V_tilde - c_tilde, 2) + pow(delta, 2)) / (2 * delta);
	}

	// Pour DeltaFc5
	if (abs(V_tilde + c_tilde) > delta) {
		double HartensCorrectionF5 = abs(V_tilde + c_tilde);
	} else {
		double HartensCorrectionF5 = (pow(V_tilde + c_tilde, 2) + pow(delta, 2)) / (2 * delta);
	}

	// 5.1 Calcul de DeltaFc1
	double termFc1 = HartensCorrectionF1 * ((pDelta - rho_tilde * c_tilde * VDelta) / (2 * pow(c_tilde, 2)));

	double rhoV_Fc1 = termFc1;
	double rhouV_Fc1 = termFc1 * (u_tilde - c_tilde * m_mesh->m_face2Normal[2 * iFace + 0]);
	double rhovV_Fc1 = termFc1 * (v_tilde - c_tilde * m_mesh->m_face2Normal[2 * iFace + 1]);
	double rhoHV_Fc1 = termFc1 * (H_tilde - c_tilde * V_tilde);

	// 5.2 Calcul de DeltaFc234
	double termFc234 = abs(V_tilde) * (rhoDelta - pDelta / pow(c_tilde, 2));

	double rhoV_Fc234 = termFc234;
	double rhouV_Fc234 = termFc234 * u_tilde + rho_tilde * (uDelta - Vdelta * m_mesh->m_face2Normal[2 * iFace + 0]);
	double rhovV_Fc234 = termFc234 * v_tilde + rho_tilde * (vDelta - Vdelta * m_mesh->m_face2Normal[2 * iFace + 1]);
	double rhoHV_Fc234 = termFc234 * q_tilde2 + rho_tilde * (u_tilde * uDelta + v_tilde * vDelta - V_tilde * VDelta);

	// 5.2 Calcul de DeltaFc5
	double termFc5 = HartensCorrectionF5 * ((pDelta - rho_tilde * c_tilde * VDelta) / (2 * pow(c_tilde, 2)));

	double rhoV_Fc5 = termFc5;
	double rhouV_Fc5 = termFc5 * (u_tilde + c_tilde * m_mesh->m_face2Normal[2 * iFace + 0]);
	double rhovV_Fc5 = termFc5 * (v_tilde + c_tilde * m_mesh->m_face2Normal[2 * iFace + 1]);
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

void Solver::DotProduct(iFace, elem1, elem2) {
	//Calcul du produit scalaire pour savoir s'il faut changer le sens de la normale a la face
	double xElem1 = m_mesh->m_element2Center[2 * elem1 + 0];
	double yElem1 = m_mesh->m_element2Center[2 * elem1 + 1];

	double xElem2 = m_mesh->m_element2Center[2 * elem2 + 0];
	double yElem2 = m_mesh->m_element2Center[2 * elem2 + 1];

	double xVecteur = xElem2 - xElem1;
	double yVecteur = yElem2 - yElem1;

	double dotProduct = xVecteur * m_mesh->m_face2Normal[2 * iFace + 0] + yVecteur * m_mesh->m_face2Normal[2 * iFace + 1];

	if (dotProduct < 0) {//TODO a verifier pour cette condition
		m_mesh->m_face2Normal[2 * iFace + 0] *= -1;
		m_mesh->m_face2Normal[2 * iFace + 1] *= -1;
	}
}
