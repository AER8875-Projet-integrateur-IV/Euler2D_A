#include "../Mesh.hpp"

class MetricsGenerator
{
protected:
	// Metrics attributes
	

public:
    MetricsGenerator(Mesh* mesh);
    ~MetricsGenerator();
	Mesh* m_mesh;
	
	void Solve();

	void SolveVolume();
	void SolveFaceVector();
	void SolveElement2Center();
	void GetTriangleCenter(double x1, double x2, double x3, double y1, double y2, double y3, double* center);
	double GetTriangleVolume(double x1, double x2, double x3, double y1, double y2, double y3);
};


