#define MESHREADER_H

#include <fstream>
#include <memory>
#include <string>

class MeshReader {
protected:
	std::ifstream *m_inFile;

	int m_nDime;
	int m_nNode;
	int m_nElement;

	std::unique_ptr<double[]> m_coor;

	std::unique_ptr<int[]> m_element2Node;
	std::unique_ptr<int[]> m_element2NodeStart;

public:
	// Methods
	void get_values(int *nDime, int *nNode, int *nElement, std::unique_ptr<double[]> *coor, std::unique_ptr<int[]> *element2Node, std::unique_ptr<int[]> *element2NodeStart);
};
