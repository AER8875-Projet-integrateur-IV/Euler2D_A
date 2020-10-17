#pragma once
#define MESHGENERATOR_H

class MeshReader;
class MeshReaderSU2;
class Mesh;

#include <string>
#include <memory>

class MeshGenerator {
	// Creates a mesh from the MeshReader classes
protected:
	// attributes needed for solving
	std::unique_ptr<int[]> m_lhelp;
	int m_nNodesForFace = 2;// In 2D a face necesseraly has 2 nodes. this needs to be changed for a 3D mesh
	int m_longueurFace2node;
	int m_nodeCount;

	std::unique_ptr<MeshReader> reader;

	// connectivity attributes
	Mesh* mesh;
	
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
	int* m_element2FaceStart;	

	std::unique_ptr<int[]> m_face2Element;
	std::unique_ptr<int[]> m_face2Node;

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
	MeshGenerator(std::string meshPath);
	~MeshGenerator();

	Mesh BuildMesh();
};