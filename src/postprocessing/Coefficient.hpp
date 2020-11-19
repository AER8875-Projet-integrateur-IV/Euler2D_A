#pragma once
#define COEFFICIENT_H

#include "../solver/Solver.hpp"
#include "../mesh/Mesh.hpp"


class Coefficient
{
private:
    Solver* m_solver;
    Mesh* m_mesh;
    std::vector<double> m_Cp;
    std::vector<double> m_x;        // position of each cp as x/chord
    double m_c=1;     // chord length
    double m_LE=0;    // chord Leading edge position

    double m_CL;
    double m_CD;
    double m_CM;

public:
    Coefficient(Solver* solver, Mesh* mesh);
    ~Coefficient();

    void Solve(int* faces, int nFace);
    void Write(std::string path);
};

