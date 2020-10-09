#include "MeshReader.hpp"
#include <memory>

void MeshReader::get_values(int *nDime, int *nNode, int *nElement, std::unique_ptr<double[]> *coor, std::unique_ptr<int[]> *element2Node, std::unique_ptr<int[]> *element2NodeStart) {
	*nDime = m_nDime;
	*nNode = m_nNode;
	*nElement = m_nElement;

	swap(*coor, m_coor);
	swap(*element2Node, m_element2Node);
	swap(*element2NodeStart, m_element2NodeStart);
}