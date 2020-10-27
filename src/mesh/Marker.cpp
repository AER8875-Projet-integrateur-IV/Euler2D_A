#include "Marker.hpp"
#include <string>
#include <memory>
#include "../utils/logger/Logger.hpp"

Marker::Marker()
{
}

Marker::~Marker()
{
	delete[] m_element2Node;
	delete[] m_element2NodeStart;
	delete[] m_element2VTKId;
	delete[] m_containingFaces;
}

void Marker::Set(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* m_element2VTKId){
	m_tag = *tag;
	m_nElement= *nElement;
	m_element2Node = element2Node;
	m_element2NodeStart = element2NodeStart;
	m_element2VTKId = m_element2VTKId;
	m_containingFaces = new int[*nElement];
	m_containingFacesNextEmptyIndex = 0;
	Logger::getInstance()->AddLog(*tag + " marker created with " + std::to_string(m_nElement) + " border elements",1);
}

int Marker::GetNElement()
{
	return m_nElement;
}

int Marker::Check4Face(int* nodes, int nnode, int nface){
	int* bNodes;
	int bNodeEndIndex, bNodeIndex;
	int bNNode;
	int match;
	for (int iElement = 0; iElement < m_nElement; iElement++)
	{
		bNodeIndex = m_element2NodeStart[iElement];
		bNodeEndIndex=m_element2NodeStart[iElement+1];
		bNNode = bNodeEndIndex-bNodeIndex;
		if (nnode != bNNode){
			continue;
		}
		bNodes = m_element2Node+bNodeIndex;

		// Check array equality
		match = this->AreArraysEqual(nodes, bNodes, bNNode);
		
	}
	if(match == 0){
		m_containingFaces[m_containingFacesNextEmptyIndex] = nface;
		return 0;
	} else{
		return 0;
	}
	
}
int Marker::AreArraysEqual(int* ar1, int* ar2, int length){
	int val1, val2;
	int match = 0;
	val2 = ar2[0];
	for (int i1 = 0; i1 < length; i1++)
	{
		val1 = ar1[i1];
		if(val1==val2){
			match++;
			for (int i2 = 1; i2 < length; i2++){
				val2 = ar2[i2];
				i1++;
				if(i1 == length){
					i1 = 0;
				} 
				val1 = ar1[i1];
				if(val1 == val2){
					match++;
				}
			}
		}
	}
	if(match == length){
		return 0;
	} else{
		return 1;
	}
}


void Marker::Update(){

}
void Marker::Update_farfield(){

}
void Marker::Update_wall(){

}
