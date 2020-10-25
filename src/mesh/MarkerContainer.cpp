#include "MarkerContainer.hpp"
#include "Marker.hpp"
#include <memory>

MarkerContainer::MarkerContainer(int nMark)
{
    m_nMark = nMark;
    m_markers = std::unique_ptr<Marker[]>(new Marker[nMark]);
    m_nextEmptyMark = 0;
}

MarkerContainer::~MarkerContainer()
{
}

void MarkerContainer::AddMarker(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart)
{
    (m_markers[m_nextEmptyMark]).Set(tag, nElement, element2Node, element2NodeStart);
    m_nextEmptyMark++;
}

int MarkerContainer::GetNElement()
{
    int nElement = 0;
    for(int i=0;i<m_nMark;i++){
        nElement = nElement + m_markers[i].GetNElement();
    }
    return nElement;
}