#pragma once
#define RESIDUAL_H

#include <vector>

class Residual
{
private:
    int m_nElement;
    int m_nIteration;

    double* m_rho;
    double* m_u;
    double* m_v;
    double* m_E;

    std::vector<double> m_rhoRMS;
    std::vector<double> m_uRMS;
    std::vector<double> m_vRMS;
    std::vector<double> m_ERMS;

public:
    Residual();
    Residual(int nElement);
    ~Residual();

    double MaxRMS();
    void Reset();
    void solveRMS();

    inline double* GetRho(int iElement){
        return m_rho+iElement;
    }

    inline double* GetU(int iElement){
        return m_u+iElement;
    }

    inline double* GetV(int iElement){
        return m_v+iElement;
    }

    inline double* GetE(int iElement){
        return m_E+iElement;
    }
};
