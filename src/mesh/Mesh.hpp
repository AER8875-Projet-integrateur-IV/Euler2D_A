#ifndef MESH_H
#define MESH_H

#include <string>

class Mesh {
protected:
	int m_nDime;
	int m_nNode;
	int m_nElement;
	int m_nFace;

	double *m_coor;

	int *m_element2Node;
	int *m_element2NodeStart;

	int *m_node2Element;
	int *m_node2ElementStart;

	int *m_element2Element;
	int *m_element2ElementStart;

	int *m_element2Face;
	int *m_element2FaceStart;

	int *m_face2Element;
	int *m_face2ElementStart;

	int *m_face2Node;
	int *m_face2Node;

public:
	// constructor and destructor
	Mesh(std::string mesh_path);
};

#endif