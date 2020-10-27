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
	Parser* m_parser;
	
	void ReadCoord();
	void ReadConnect(int* nElement, int** element2Node, int** element2NodeStart, int** element2VTKId);
	void ReadMarks();
	int VtkElem2NNode(int vtkId);

public:
	MeshReaderSU2(std::string meshPath, Mesh* mesh);
	MeshReaderSU2();
	~MeshReaderSU2();
	void ReadFile();
};
