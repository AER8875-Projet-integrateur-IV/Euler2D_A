#pragma once
#define MESHGENERATOR_H

class MeshReader;
class MeshReaderSU2;

#include "../Mesh.hpp"
#include <memory>
#include <string>
#include "../MarkerContainer.hpp"

class MeshGenerator {
	// Creates a mesh from the MeshReader classes
protected:
	// attributes needed for solving
	std::unique_ptr<int[]> m_lhelp;
	int m_nNodesForFace = 2;// In 2D a face necesseraly has 2 nodes. this needs to be changed for a 3D mesh
	int m_longueurFace2node;
	int m_nodeCount;

	std::unique_ptr<MarkerContainer> m_markers;

	std::unique_ptr<MeshReader> reader;

	// connectivity attributes
	int m_nDime;
	int m_nNode;
	int m_nElement;
	int m_nFace;			// Number of faces without double counting
	int m_nFaceInt; 		// Number of internal faces

	std::unique_ptr<double[]> m_coor;
	std::unique_ptr<int[]> m_element2Node;
	std::unique_ptr<int[]> m_element2NodeStart;

	std::unique_ptr<int[]> m_node2Element;
	std::unique_ptr<int[]> m_node2ElementStart;

	std::unique_ptr<int[]> m_element2Element;
	std::unique_ptr<int[]> m_element2ElementStart;

	std::unique_ptr<int[]> m_element2Face;
	int *m_element2FaceStart;

	std::unique_ptr<int[]> m_face2Element;
	std::unique_ptr<int[]> m_face2Node;

	// Metrics attributes
	std::unique_ptr<double[]> m_element2Volume;
	std::unique_ptr<double[]> m_face2FaceVector;
	std::unique_ptr<double[]> m_face2Normal;
	std::unique_ptr<double[]> m_face2Area;
	std::unique_ptr<double[]> m_element2Center;


	// Methods
	void SolveNode2ElementStart();
	void SolveNode2Element();

	void SolveElement2ElementStart();
	void SolveElement2FaceStart();
	void CountFaces();
	void SolveFaceConnectivity();

	void SolveFace2Element();
	void SolveFace2Node();

	void SolveVolume();
	void SolveFaceVector();
	void SolveElement2Center();
	double *GetTriangleCenter(double x1, double x2, double x3, double y1, double y2, double y3);
	double GetTriangleVolume(double x1, double x2, double x3, double y1, double y2, double y3);

public:
	MeshGenerator();
	MeshGenerator(std::string meshPath);
	~MeshGenerator();

	Mesh BuildMesh();
};
