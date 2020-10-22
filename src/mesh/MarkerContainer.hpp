#pragma once
#define MARKERCONTAINER_H

#include<vector>
#include<memory>

class Marker;

class MarkerContainer
{
private:
    std::unique_ptr<Marker[]> m_markers;
    int m_nMark;
    int m_nextEmptyMark;

public:
    // Destructor and constructor
    MarkerContainer(int nMark);
    ~MarkerContainer();

    // methods
    void AddMarker(std::string* tag, int* nElement, std::unique_ptr<int[]>* element2Node, std::unique_ptr<int[]>* element2NodeStart);
    int GetNElement();
};

