#include "Mesh.hpp"
#include <memory>


Mesh::Mesh(int *nDime, int *nNode, int *nElement, int *nFace, int *nFaceInt,
           std::unique_ptr<double[]> *coor, std::unique_ptr<int[]> *face2Element, std::unique_ptr<int[]> *face2Node) {
	m_nDime = *nDime;
	m_nElement = *nElement;
	m_nFace = *nFace;
	m_nFaceInt = *nFaceInt;

	swap(*coor, m_coor);
	swap(*face2Element, m_face2Element);
	swap(*face2Node, m_face2Node);
}