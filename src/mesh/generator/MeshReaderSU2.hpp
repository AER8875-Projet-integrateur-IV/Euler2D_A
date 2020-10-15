#pragma once
#define MESHREADERSU2_H

#include "MeshReader.hpp"
#include "Parser.hpp"
#include <fstream>
#include <string>

class MeshReaderSU2 : public MeshReader {
protected:
	Parser *parser = NULL;
	
	void ReadCoord();
	void ReadConnect();
	void ReadMarks();
	int VtkElem2NNode(int vtkId);

public:
	MeshReaderSU2(std::string meshPath);
	void ReadFile();
};
