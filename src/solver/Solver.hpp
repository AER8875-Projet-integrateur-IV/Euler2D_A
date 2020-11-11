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
	double E;
};

struct DeltaW
// Valeur storee dans les elements
{
	double rho;
	double u;
	double v;
	double E;
};

struct Residuals
// Valeur storee dans les elements et ghost elements
{
	double rho;
	double u;
	double v;
	double E;
};

class Solver
{
private:
	Mesh *m_mesh;
	ees2d::io::InputParser* m_inputParameters;

    // pointer to the chosen calculation scheme
    void (Solver::*m_scheme)(int iFace);

public:
	Fc *m_face2Fc;
	W* m_element2W;
    W* m_Winf; // initial values
	Residuals *m_element2Residuals;
	DeltaW *m_element2DeltaW;

	Solver(Mesh* mesh, ees2d::io::InputParser* IC);
    ~Solver();

	// Convective Flux
	void SolveFc();

    // Average scheme
	void ConvectiveFluxAverageScheme(int iFace);

	// Row scheme
    void ConvectiveFluxRoeScheme(int iFace);

	// Solver itself
	// Dot Product
	void DotProduct(int iFace,int elem1,int  elem2);
	// TimeStep
	double LocalTimeStep(int iElem);
	// Residuals
	void CalculateResiduals(int iFace);
	// Delta W
	void CalculateDeltaW(int iElem, double LocalTimeStep);
	// Update W
	void UpdateW(int iElem);
};
