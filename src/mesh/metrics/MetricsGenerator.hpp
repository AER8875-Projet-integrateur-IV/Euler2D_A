#include "../Mesh.hpp"

class MetricsGenerator
{
protected:
	// Metrics attributes
	Mesh* m_mesh;

public:
    MetricsGenerator(Mesh* mesh);
    ~MetricsGenerator();

	void Solve(){
		this->SolveVolume();
		this->SolveFaceVector();
		this->SolveElement2Center();
	}
	void SolveVolume();
	void SolveFaceVector();
	void SolveElement2Center();
	void GetTriangleCenter(double x1, double x2, double x3, double y1, double y2, double y3, double* center);
	double GetTriangleVolume(double x1, double x2, double x3, double y1, double y2, double y3);
};


