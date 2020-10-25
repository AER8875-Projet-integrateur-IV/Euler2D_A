#pragma once
#define MESHREADER_H

#include "../Mesh.hpp"
#include <fstream>
#include <memory>
#include <string>

class MeshReader {
	// Interface between MeshGenerator and the reader classes
protected:
	Mesh* m_mesh;

public:
	// Methods
	virtual void ReadFile();
};
