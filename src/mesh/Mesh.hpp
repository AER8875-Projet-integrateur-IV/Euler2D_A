#pragma once
#define MESH_H

#include <memory>
#include <string>
#include "MarkerContainer.hpp"

class Mesh {
public:
	// Connectivité
	int m_nDime;
	int m_nNode;
	int m_nElement;
	int m_nFace;   			// Number of faces without double counting
	int m_nFaceInt;			// Number of internal faces

	double* m_coor;
	int* m_element2Node;
	int* m_element2NodeStart;	
	
	int* m_element2VTKId; 	// Stores type of element according to VTK format

	int* m_node2Element;
	int* m_node2ElementStart;	
	
	int* m_element2Element;
	int* m_element2ElementStart;	

	int* m_element2Face;
	int* m_element2FaceStart;	

	int* m_face2Element;
	int* m_face2Node;

	// Markers
	MarkerContainer* m_markers;

	// metrics
	double* m_element2Volume;
	double* m_face2FaceVector;
	double* m_face2Normal;
	double* m_face2Area;
	double* m_element2Center;	

	// constructor and destructor
	Mesh();
	~Mesh();

	//void Element2NodeVectors(int iElement, int nLocalNode, double* v);
};
