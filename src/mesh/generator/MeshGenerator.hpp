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
	// attributes
	std::unique_ptr<MeshReader> reader;

	int m_nDime;
	int m_nNode;
	int m_nElement;
	std::unique_ptr<double[]> m_coor;
	std::unique_ptr<int[]> m_element2Node;
	std::unique_ptr<int[]> m_element2NodeStart;	

	std::unique_ptr<int[]> m_node2Element;
	std::unique_ptr<int[]> m_node2ElementStart;	


	// Methods
	void CreateNode2Element();
	void CreateElement2Element();
	void CreateElement2Face();
	void CreateFace2Element();
	void CreateFace2Node();
	void CreateGhostElement();
	void CreateMarker();

public:
	MeshGenerator(std::string meshPath);
	~MeshGenerator();

	Mesh BuildMesh();
};