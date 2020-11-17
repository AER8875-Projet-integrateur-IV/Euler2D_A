#pragma once
#define SOLVER_H

#include"../mesh/Mesh.hpp"
#include"../inputParser/InputParser.h"
#include"Residual.hpp"

struct Fc
// Valeur storée dans les faces
{
    double rho; 		// rho*V
    double u;			// rho*V*u
    double v;			// rho*V*v
	double H;			// rho*V*H
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

class Solver
{
private:
	// pointer to the chosen calculation scheme
    void (Solver::*m_scheme)(int iFace);

public:
	ees2d::io::InputParser* m_inputParameters;
    Mesh *m_mesh;
	Fc *m_face2Fc;
	W* m_element2W;
    W* m_Winf; // initial values
	Residual* m_element2Residuals;
	DeltaW *m_element2DeltaW;

	double m_Vref;

	Solver(Mesh* mesh, ees2d::io::InputParser* IC);
    ~Solver();

	// Convective Flux
	void SolveFc();

    // Average scheme
	void ConvectiveFluxAverageScheme(int iFace);

	// Row scheme
    void ConvectiveFluxRoeScheme(int iFace);

	// Calcul des flux aux BC
	void BCFlux(int iFace);

	// Solver itself
	// Dot Product
	void DotProduct(int iFace);
	void DotProductBC(int iFace);
	// TimeStep
	double LocalTimeStep(int iElem);
	// Residuals
	void CalculateResiduals(int iFace);
	void CalculateResidualsBC(int iFace);
	// Delta W
	void CalculateDeltaW(int iElem, double LocalTimeStep);
	// Update W
	void UpdateW(int iElem);

	double solveE(double p, double gamma, double rho, double u, double v, double w);
};
