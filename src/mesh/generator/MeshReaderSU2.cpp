#include "MeshReaderSU2.hpp"
#include "Parser.hpp"
#include "../../utils/logger/Logger.cpp"
#include "../MarkerContainer.hpp"

#include <fstream>
#include <iostream>
#include <limits>// std::numeric_limits
#include <memory>//declarations of unique_ptr
#include <regex>
#include <sstream>
#include <sstream>// std::stringstream
#include <string>
#include <stdexcept>

MeshReaderSU2::MeshReaderSU2(std::string meshPath) {
	m_parser = std::unique_ptr<Parser>(new Parser(meshPath, "%"));
}

void MeshReaderSU2::ReadFile() {
	std::string message;

	m_nDime = m_parser->ExtractNextInt();
	message = ("number of dimensions in the mesh= "+ std::to_string(m_nDime));
	Logger::getInstance()->AddLog(message,1);

	std::string word;
	for (int i=0;i<3;i++){
		word = m_parser->GetNextWord();
		if (word == "NELEM="){
			m_nElement = m_parser->ExtractNextInt();
			Logger::getInstance()->AddLog("number of elements in the mesh= " + std::to_string(m_nElement),1);
			this->ReadConnect(&m_nElement,&m_element2Node,&m_element2NodeStart);

		} else if (word == "NPOIN=")
		{
			m_nNode = m_parser->ExtractNextInt();
			Logger::getInstance()->AddLog("number of points in the mesh= " + std::to_string(m_nNode),1);

			MeshReaderSU2::ReadCoord();
		} else if (word == "NMARK=")
		{
			this->ReadMarks();
		} else
		{
			throw std::invalid_argument("SU2 file has an invalid command");
		}
		
		
	}
}

void MeshReaderSU2::ReadCoord() {
	double val;

	// Creation of the coordinates array
	m_coor = std::make_unique<double[]>(m_nNode * m_nDime);

	// Filling of the coordinates array
	for (int inode = 0; inode < m_nNode; inode++) {
		for (int idim = 0; idim < m_nDime; idim++) {
			val = m_parser->ExtractNextDouble();
			m_coor[inode * m_nDime + idim] = val;
			//std::cout << val << std::endl;
		}
		m_parser->m_inFile.unget();
		m_parser->GetNextNonNullLine();
	}
}

int MeshReaderSU2::VtkElem2NNode(int VTKid) {
	int number_of_nodes;
	switch (VTKid) {
		case 3:
			number_of_nodes = 2;
			break;

		case 5:
			number_of_nodes = 3;
			break;

		case 9:
			number_of_nodes = 4;
			break;

		case 10:
			number_of_nodes = 4;
			break;

		case 12:
			number_of_nodes = 8;
			break;

		case 13:
			number_of_nodes = 6;
			break;

		case 14:
			number_of_nodes = 5;
			break;
	}

	return number_of_nodes;
}

void MeshReaderSU2::ReadConnect(int* nElement, std::unique_ptr<int[]>* element2Node, std::unique_ptr<int[]>* element2NodeStart) {
	int val;
	int filePosition = m_parser->m_inFile.tellg();
	int element2NodeSize = 0;
	int nNode;

	// Counting size of connectivity matrix
	for (int ielem = 0; ielem < (*nElement); ielem++) {
		val = m_parser->ExtractNextInt();
		nNode = MeshReaderSU2::VtkElem2NNode(val);
		element2NodeSize += nNode;
		m_parser->m_inFile.unget();
		m_parser->GetNextNonNullLine();
	}
	m_parser->m_inFile.seekg(filePosition);


	// Filling of the connectivity matrix
	element2Node->reset(new int[element2NodeSize]);
	element2NodeStart->reset(new int[(*nElement)+1]);

	int element2NodeCurrentStart = 0;

	(element2NodeStart->get())[0] = 0;
	for (int ielem = 0; ielem < (*nElement); ielem++) {
		

		val = m_parser->ExtractNextInt();
		nNode = MeshReaderSU2::VtkElem2NNode(val);

		for (int iNode = 0; iNode < nNode; iNode++) {
			val = m_parser->ExtractNextInt();
			(element2Node->get())[element2NodeCurrentStart] = val;
			element2NodeCurrentStart++;
		}
		m_parser->m_inFile.unget();
		m_parser->GetNextNonNullLine();
		(element2NodeStart->get())[ielem+1] = element2NodeCurrentStart;
	}
}

void MeshReaderSU2::ReadMarks() {
	int nMark = m_parser->ExtractNextInt();
	Logger::getInstance()->AddLog("number of markers in the mesh= " + std::to_string(nMark),1);

	m_markers = std::unique_ptr<MarkerContainer>(new MarkerContainer(nMark));
	std::string tag;
	int nElement;
	std::unique_ptr<int[]> element2Node;
	std::unique_ptr<int[]> element2NodeStart;
	
	for(int iMark = 0; iMark<nMark;iMark++){
		m_parser->GetNextWord();
		tag = m_parser->GetNextWord();
		m_parser->GetNextWord();
		nElement = m_parser->ExtractNextInt();

		this->ReadConnect(&nElement,&element2Node,&element2NodeStart);

		m_markers->AddMarker(&tag, &nElement, &element2Node, &element2NodeStart);
	}

}
