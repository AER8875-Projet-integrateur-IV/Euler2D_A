#pragma once
#define MESHREADER_H

#include "../MarkerContainer.hpp"
#include <fstream>
#include <memory>
#include <string>

class MeshReader {
	// Interface between MeshGenerator and the reader classes
protected:
	int m_nDime;
	int m_nNode;
	int m_nElement;

	std::unique_ptr<double[]> m_coor;

	std::unique_ptr<int[]> m_element2Node;
	std::unique_ptr<int[]> m_element2NodeStart;

	std::unique_ptr<MarkerContainer> m_markers;

public:
	// Methods
	virtual void ReadFile();
	void get_values(int *nDime, int *nNode, int *nElement, std::unique_ptr<double[]> *coor, std::unique_ptr<int[]> *element2Node, std::unique_ptr<int[]> *element2NodeStart);
	void get_markers(std::unique_ptr<MarkerContainer>* m_markers);
};
