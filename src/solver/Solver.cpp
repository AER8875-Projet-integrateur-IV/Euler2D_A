#include "Solver.hpp"

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
    // Calcul Fc interne
    for(int iFace = 0;iFace<m_mesh->m_nFaceNoBoundaries;iFace++){
        // Calcul Fc

		// Schema avec les moyennes
		elem1 = m_mesh->m_face2Element[2 * iFace + 0];
		elem2 = m_mesh->m_face2Element[2 * iFace + 1];

		rho_avg = 0.5 * (solver->m_element2W.rho[elem1] + solver->m_element2W.rho[elem2]);
		u_avg = 0.5 * (solver->m_element2W.u[elem1] + solver->m_element2W.u[elem2]);
		v_avg = 0.5 * (solver->m_element2W.v[elem1] + solver->m_element2W.v[elem2]);
		H_avg = 0.5 * (solver->m_element2W.H[elem1] + solver->m_element2W.H[elem2]);
		P_avg = 0.5 * (solver->m_element2W.P[elem1] + solver->m_element2W.P[elem2]);

		// Calcul de la vitesse contravariante
		V = u_avg * m_mesh->m_face2Normal[2 * iFace + 0] + v_avg * m_mesh->m_face2Normal[2 * iface + 1];

		// Calcul de Fc pour chaque face interne
		// rho
		solver->m_face2Fc.rho[iFace] = rho_avg * V;
		//u
		solver->m_face2Fc.u[iFace] = rho_avg * u_avg * V + m_mesh->m_face2Normal[2 * iFace + 0] * p_avg;
		//v
		solver->m_face2Fc.v[iFace] = rho_avg * v_avg * V + m_mesh->m_face2Normal[2 * iFace + 1] * p_avg;
		//p
		solver->m_face2Fc.H[4 * iFace + 3] = rho_avg * H_avg * V;


		// Schema de Roe
	}

    // m_mesh->m_markers.update()
}
