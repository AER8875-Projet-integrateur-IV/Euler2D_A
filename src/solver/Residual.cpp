#include "Residual.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

Residual::Residual()
{
    m_nElement = 0;
    m_rho = NULL;
    m_u = NULL;
    m_v = NULL;
    m_E = NULL; 
}

Residual::Residual(int nElement)
{
    m_nElement = nElement;
    m_rho = new double[nElement];
    m_u = new double[nElement];
    m_v = new double[nElement];
    m_E = new double[nElement];
}

Residual::~Residual()
{
    delete[] m_rho;
    delete[] m_u;
    delete[] m_v;
    delete[] m_E;
}

void Residual::solveRMS(){
    double R1, R2, R3, R4 = 0;
    for(int iElement = 0;iElement<m_nElement;iElement++){
        R1 += pow(m_rho[iElement], 2);
        R2 += pow(m_u[iElement], 2);
        R3 += pow(m_v[iElement], 2);
        R4 += pow(m_E[iElement], 2);
    } 
    m_rhoRMS.push_back(pow(R1/m_nElement,0.5)); 
    m_uRMS.push_back(pow(R2/m_nElement,0.5));
    m_vRMS.push_back(pow(R3/m_nElement,0.5));
    m_ERMS.push_back(pow(R4/m_nElement,0.5));
}

double Residual::MaxRMS(){
    int n = m_uRMS.size();
    return std::max({m_rhoRMS[n-1], 
                     m_uRMS[n-1], 
                     m_vRMS[n-1], 
                     m_ERMS[n-1]});
}

void Residual::Reset(){
    for(int iElement = 0;iElement<m_nElement;iElement++){
        m_rho[iElement] = 0;
        m_u[iElement] = 0;
        m_v[iElement] = 0;
        m_E[iElement] = 0;
    }
}