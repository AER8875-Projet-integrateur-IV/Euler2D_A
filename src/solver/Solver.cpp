#include "Solver.hpp"
#include <cmath>
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

	double E = IC->m_Pressure/IC->m_Density/IC->m_Gamma+0.5*pow(IC->m_Mach,2);
	m_Winf->H=E+IC->m_Pressure/IC->m_Density;
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
	m_face2Fc = new Fc[m_mesh->m_nFace];


	// initialise markers
	Logger::getInstance()->AddLog("Defining the type of the border conditions face elements...",1);
	m_mesh->m_markers->Check4Face(m_mesh->m_face2Node, m_mesh->m_nFace);
	Logger::getInstance()->AddLog("Defining the type of the border conditions ghost cells...",1);
	m_mesh->m_markers->FindElements(m_mesh);
}

Solver::~Solver()
{
	delete[] m_element2W;
	delete[] m_face2Fc;
	delete m_Winf;
}


int elem1, elem2;
double rho_avg, u_avg, v_avg, H_avg, V;

void Solver::SolveFc(){
    // Calcul Fc interne
    for(int iFace = 0;iFace<m_mesh->m_nFaceNoBoundaries;iFace++){
        // // Calcul Fc

		// // Schema avec les moyennes
		// elem1 = m_mesh->m_face2Element[2 * iFace + 0];
		// elem2 = m_mesh->m_face2Element[2 * iFace + 1];

		// rho_avg = 0.5 * (solver->m_element2W.rho[elem1] + solver->m_element2W.rho[elem2]);
		// u_avg = 0.5 * (solver->m_element2W.u[elem1] + solver->m_element2W.u[elem2]);
		// v_avg = 0.5 * (solver->m_element2W.v[elem1] + solver->m_element2W.v[elem2]);
		// H_avg = 0.5 * (solver->m_element2W.H[elem1] + solver->m_element2W.H[elem2]);
		// P_avg = 0.5 * (solver->m_element2W.P[elem1] + solver->m_element2W.P[elem2]);

		// // Calcul de la vitesse contravariante
		// V = u_avg * m_mesh->m_face2Normal[2 * iFace + 0] + v_avg * m_mesh->m_face2Normal[2 * iface + 1];

		// // Calcul de Fc pour chaque face interne
		// // rho
		// solver->m_face2Fc.rho[iFace] = rho_avg * V;
		// //u
		// solver->m_face2Fc.u[iFace] = rho_avg * u_avg * V + m_mesh->m_face2Normal[2 * iFace + 0] * p_avg;
		// //v
		// solver->m_face2Fc.v[iFace] = rho_avg * v_avg * V + m_mesh->m_face2Normal[2 * iFace + 1] * p_avg;
		// //p
		// solver->m_face2Fc.H[4 * iFace + 3] = rho_avg * H_avg * V;


		// Schema de Roe
	}

    // m_mesh->m_markers.update()
}
