#pragma once
#define RESIDUAL_H

#include <string>

class Residual
{
private:
    int m_nElement;
    int m_iteration;
    int m_maxIter;

    double* m_rho;
    double* m_u;
    double* m_v;
    double* m_E;

    double* m_rhoRMS;
    double* m_uRMS;
    double* m_vRMS;
    double* m_ERMS;

public:
    Residual();
    Residual(int nElement, int maxIter);
    ~Residual();

    double MaxRMS();
    void Reset();
    void solveRMS();
    void Write2File(std::string path);

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
