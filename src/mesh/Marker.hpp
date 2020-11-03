#pragma once
#define MARKER_H

#include <string>
#include <memory>

class Marker
{
protected:
    std::string m_tag;
    int m_nElement;
    int* m_element2Node;
    int* m_element2NodeStart;
    int* m_element2VTKId;
    int* m_containingFaces;
    int m_containingFacesNextEmptyIndex;

public:
    Marker();
    ~Marker();

    void Set(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* element2VTKId);
    int GetNElement();

    // Returns 0 if a match has been found, 1 otherwise
    int Check4Face(int* nodes, int nnode, int nface);
    int AreArraysEqual(int* ar1,int* ar2, int length);

    void Update();
    void Update_farfield();
    void Update_wall();
};

