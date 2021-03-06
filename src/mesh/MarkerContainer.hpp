#pragma once
#define MARKERCONTAINER_H

#include<vector>
#include<memory>

class Mesh;
class Marker;
class Solver;

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
    void AddMarker(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* element2VTKId);
    void Check4Face(int* face2Node, int nFace);
    void FindElements(Mesh* mesh);
    int GetNElement();
    void Update(Mesh* mesh, Solver* solver);
};

