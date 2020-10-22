#include "Marker.hpp"
#include <string>
#include <memory>
#include "../utils/logger/Logger.hpp"

Marker::Marker()
{
}

Marker::~Marker()
{
}

void Marker::Set(std::string* tag, int* nElement, std::unique_ptr<int[]>* element2Node, std::unique_ptr<int[]>* element2NodeStart)
{
    m_tag = *tag;
	m_nElement = *nElement;

	std::swap(m_element2Node, *element2Node);
	std::swap(m_element2NodeStart, *element2NodeStart);  
	Logger::getInstance()->AddLog(*tag + " marker created with " + std::to_string(m_nElement) + " border elements",1);
}

int Marker::GetNElement()
{
	return m_nElement;
}