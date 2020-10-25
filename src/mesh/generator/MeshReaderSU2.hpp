#pragma once
#define MESHREADERSU2_H

#include "Parser.hpp"
#include "MeshReader.hpp"
#include <fstream>
#include <string>
#include <memory>

class MeshReaderSU2 : public MeshReader {
	// Read SU2 format Mesh
protected:
	std::unique_ptr<Parser> m_parser;
	
	void ReadCoord();
	void ReadConnect(int* nElement, int* element2Node, int* element2NodeStart);
	void ReadMarks();
	int VtkElem2NNode(int vtkId);

public:
	MeshReaderSU2(std::string meshPath, Mesh* mesh);
	void ReadFile();
};
