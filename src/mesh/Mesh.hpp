#pragma once
#define MESH_H

#include <memory>
#include <string>

class Mesh {
protected:
	int m_nDime;
	int m_nNode;
	int m_nElement;
	int m_nFace;   // Number of faces without double counting
	int m_nFaceInt;// Number of internal faces

	std::unique_ptr<double[]> m_coor;

	std::unique_ptr<int[]> m_element2Face;
	// m_element2FaceStart = m_element2Face;

	std::unique_ptr<int[]> m_face2Element;
	std::unique_ptr<int[]> m_face2Node;

public:
	// constructor and destructor
	Mesh(int *nDime, int *nNode, int *nElement, int *nFace, int *nFaceInt,
	     std::unique_ptr<double[]> *coor, std::unique_ptr<int[]> *face2Element, std::unique_ptr<int[]> *face2Node);
};
