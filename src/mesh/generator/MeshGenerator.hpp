#pragma once
#define MESHGENERATOR_H

#include "MeshReader.hpp"
#include <string>
#include "../Mesh.hpp"

class MeshGenerator {
protected:
	MeshReader* reader;

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