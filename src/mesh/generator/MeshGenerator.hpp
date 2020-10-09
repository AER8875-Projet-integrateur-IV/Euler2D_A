#pragma once
#define MESHGENERATOR_H

#include "MeshReader.hpp"
#include <string>

class MeshGenerator {
protected:
	MeshReader *Reader;

	// Methods
	void CreateNode2Element();
	void CreateElement2Element();
	void CreateElement2Face();
	void CreateFace2Element();
	void CreateFace2Node();
	void CreateGhostElement();
	void CreateMarker();

public:
	void ConstructMesh(std::string mesh_path);
};