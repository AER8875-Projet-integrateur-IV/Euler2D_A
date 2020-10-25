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

	Mesh* m_mesh;

	// Methods
	void SolveNode2ElementStart();
	void SolveNode2Element();

	void SolveElement2ElementStart();
	void SolveElement2FaceStart();
	void CountFaces();
	void SolveFaceConnectivity();

	void SolveFace2Element();
	void SolveFace2Node();
	
public:
	MeshGenerator();
	MeshGenerator(Mesh* mesh);
	~MeshGenerator();

	void BuildMesh();
};
