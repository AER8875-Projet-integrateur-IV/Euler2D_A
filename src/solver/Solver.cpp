#include "Solver.hpp"

Solver::Solver(Mesh* mesh, InitialConditions* IC)
{
    m_mesh = mesh;

    // Initialisation
}

Solver::~Solver()
{
}

void Solver::SolveFc(){
    // Calcul Fc interne
    for(int iFace = 0;iFace<m_mesh->m_nFaceNoBoundaries;iFace++){
        // Calcul Fc
    }

    // m_mesh->m_markers.update()
}