#include "Residual.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

Residual::Residual()
{
    m_nElement = 0;
    m_iteration = 0;
    m_maxIter = 0;
    m_rho = new double[m_nElement];
    m_u = new double[m_nElement];
    m_v = new double[m_nElement];
    m_E = new double[m_nElement];

    m_rhoRMS = new double[m_maxIter];
    m_uRMS = new double[m_maxIter];
    m_vRMS = new double[m_maxIter];
    m_ERMS = new double[m_maxIter];
}

Residual::Residual(int nElement, int maxIter)
{
    m_nElement = nElement;
    m_iteration = 0;
    m_maxIter = maxIter;
    m_rho = new double[nElement];
    m_u = new double[nElement];
    m_v = new double[nElement];
    m_E = new double[nElement];

    m_rhoRMS = new double[maxIter];
    m_uRMS = new double[maxIter];
    m_vRMS = new double[maxIter];
    m_ERMS = new double[maxIter];
}

Residual::~Residual()
{
    delete[] m_rho;
    delete[] m_u;
    delete[] m_v;
    delete[] m_E;

    delete[] m_rhoRMS;
    delete[] m_uRMS;
    delete[] m_vRMS;
    delete[] m_ERMS;
}

void Residual::solveRMS(){
    double R1, R2, R3, R4;
    R1 = 0;
    R2 = 0;
    R3 = 0;
    R4 = 0;
    for(int iElement = 0;iElement<m_nElement;iElement++){
        R1 += pow(m_rho[iElement], 2);
        R2 += pow(m_u[iElement], 2);
        R3 += pow(m_v[iElement], 2);
        R4 += pow(m_E[iElement], 2);
    } 
    m_rhoRMS[m_iteration] = (pow(R1/m_nElement,0.5)); 
    m_uRMS[m_iteration] = (pow(R2/m_nElement,0.5));
    m_vRMS[m_iteration] = (pow(R3/m_nElement,0.5));
    m_ERMS[m_iteration] = (pow(R4/m_nElement,0.5));
}

double Residual::MaxRMS(){
    return std::max({m_rhoRMS[m_iteration], 
                     m_uRMS[m_iteration], 
                     m_vRMS[m_iteration], 
                     m_ERMS[m_iteration]});
}

void Residual::Reset(){
    for(int iElement = 0;iElement<m_nElement;iElement++){
        m_rho[iElement] = 0;
        m_u[iElement] = 0;
        m_v[iElement] = 0;
        m_E[iElement] = 0;
    }
}