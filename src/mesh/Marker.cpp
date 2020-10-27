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
}

void Marker::Set(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* m_element2VTKId){
	m_tag = *tag;
	m_nElement= *nElement;
	m_element2Node = element2Node;
	m_element2NodeStart = element2NodeStart;
	m_element2VTKId = m_element2VTKId;
	Logger::getInstance()->AddLog(*tag + " marker created with " + std::to_string(m_nElement) + " border elements",1);
}

int Marker::GetNElement()
{
	return m_nElement;
}