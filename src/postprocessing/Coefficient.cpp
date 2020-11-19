#include "Coefficient.hpp"

#include <string>
#include <vector>
#include <cmath>

#include "../solver/Solver.hpp"
#include "../mesh/Mesh.hpp"
#include "../utils/logger/Logger.hpp"

Coefficient::Coefficient(Solver* solver, Mesh* mesh)
{
    m_solver = solver;
    m_mesh = mesh;    
}

Coefficient::~Coefficient()
{
}

void Coefficient::Solve(int* faces, int nFace){
    int iFace;
    double faceVectorX, faceVectorY;
    int elem1, elem2;
    int node1, node2;
    double x1, x2, y1, y2;          // face nodes coordinates
    double xc, yc;                  // face middle point
    double xelem, yelem;            // element center
    double xout, yout;              // vector going from the element center to the center of the boundary face
    double orientation;
    double p;                       // pressure in the cell
    double pinf, rhoinf, vinf_sqr;  // freestream airflow parameters
    
    m_CD += 0;
    m_CL += 0;
    for(int i = 0;i<nFace;i++){
        iFace = faces[i];
        
        faceVectorX = m_mesh->m_face2FaceVector[2*iFace+0];
        faceVectorY = m_mesh->m_face2FaceVector[2*iFace+1];

        elem1 = m_mesh->m_face2Element[2*iFace+0];
        elem2 = m_mesh->m_face2Element[2*iFace+1];

        // solve CP
        p = m_solver->m_element2W[elem1].P;
        pinf = m_solver->m_Winf->P;
        rhoinf = m_solver->m_Winf->rho;
        vinf_sqr = pow(m_solver->m_Winf->u,2)+pow(m_solver->m_Winf->v,2);
        m_Cp.push_back((p-pinf)/(0.5*rhoinf*vinf_sqr));
        
        // check face vector orientation
        node1 = m_mesh->m_face2Node[2*iFace+0];
        node2 = m_mesh->m_face2Node[2*iFace+1];

        x1 = m_mesh->m_coor[2*node1+0];
        y1 = m_mesh->m_coor[2*node1+1];
    
        x2 = m_mesh->m_coor[2*node2+0];
        y2 = m_mesh->m_coor[2*node2+1];

        xc = (x1+x2)/2;
        yc = (y1+y2)/2;

        xelem = m_mesh->m_element2Center[2*elem1+0];
        yelem = m_mesh->m_element2Center[2*elem1+1];
    
        xout = xc-xelem;
        yout = yc-yelem;

        orientation = xout*faceVectorX+yout*faceVectorY;
        if(orientation < 0){
            // face vector pointing out of the airfoil
            faceVectorX *= -1;
            faceVectorY *= -1;        
        }
        m_x.push_back(xc);
        m_CD += m_Cp.back()*faceVectorX;
        m_CL += m_Cp.back()*faceVectorY;
    }
    Logger::getInstance()->AddLog("CL = "+std::to_string(m_CL),0);
    Logger::getInstance()->AddLog("CD = "+std::to_string(m_CD),0);
}