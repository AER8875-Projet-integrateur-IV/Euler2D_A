#pragma once
#define SOLVER_H

#include"../mesh/Mesh.hpp"
#include"../InputParser.h"

struct Fc
// Valeur storée dans les faces
{
    double rho;
    double u;
    double v;
	double H;
};

struct W
// Valeur storée dans les éléments et ghost elements
{
    W() : P(0), H(0), rho(0), u(0), v(0) {}

	double rho;
	double u;
	double v;
	double P;
	double H;
};

class Solver
{
private:
    Mesh* m_mesh; 
    ees2d::io::InputParser* m_inputParameters;

public:
    Fc* m_face2Fc; 
    W* m_element2W;
    W* m_Winf; // initial values

    Solver(Mesh* mesh, ees2d::io::InputParser* IC);
    ~Solver();

    void SolveFc();
	void ConvectiveFluxAverageScheme(int iFace);
	void ConvectiveFluxRoeScheme(int iFace);
	void DotProduct(int iFace,int elem1,int  elem2);
};
