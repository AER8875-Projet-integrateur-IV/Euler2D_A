#pragma once
#define MARKER_H

#include <string>
#include <memory>
#include "Mesh.hpp"
#include "../solver/Solver.hpp"

class Marker
{
protected:


public:
    std::string m_tag;
    int m_nElement;
    int* m_element2Node;
    int* m_element2NodeStart;
    int* m_element2VTKId;

    // array containing the index of the border faces
    int* m_containingFaces;
    int m_containingFacesNextEmptyIndex;

    // array containing the index of each border element
    int* m_containingElement;
    // array containing the index of each border ghost element
    int* m_ghostElement;

    // member function used for border conditoin update
    void (Marker::*m_updateFunction)(Mesh* mesh, Solver* solver, int index);

    Marker();
    ~Marker();

    void Set(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* element2VTKId);
    int GetNElement();

    void DefineUpdateFunction(std::string borderType);

    // Returns 0 if a match has been found, 1 otherwise
    int Check4Face(int* nodes, int nnode, int nface);
    int AreArraysEqual(int* ar1,int* ar2, int length);
    void FindElements(Mesh* mesh);

    void Update(Mesh* mesh, Solver* solver);
    void Update_farfield(Mesh* mesh, Solver* solver, int iFace);
    void Update_wall(Mesh* mesh, Solver* solver, int iFace);

	void VortexCorrection(Mesh *mesh, Solver *solver, int index);
};

