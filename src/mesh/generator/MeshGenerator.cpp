#include "MeshGenerator.hpp"
#include "MeshReaderSU2.hpp"
#include "../Mesh.hpp"
#include "../../utils/logger/Logger.hpp"
#include "../MarkerContainer.hpp"
#include <cmath>

MeshGenerator::MeshGenerator(){
}

MeshGenerator::MeshGenerator(Mesh* mesh){
	m_mesh = mesh;
}

MeshGenerator::~MeshGenerator(){
    //empty
}

void MeshGenerator::BuildMesh(){
    // Read values from input file
	this->CountFaces();

	this->SolveNode2ElementStart();
	this->SolveNode2Element();
	this->SolveElement2ElementStart();
	this->SolveFaceConnectivity();
	this->SolveFace2Element();
	this->SolveFace2Node();

}

void MeshGenerator::SolveNode2ElementStart(){
	int startI;
	int endI;
	int nodeI;
	int j;

    m_mesh->m_node2ElementStart = new int[m_mesh->m_nNode+1]();
    // Looping over the elements to count the number of elements around each nodes
	for (int elemi=0;elemi<m_mesh->m_nElement;++elemi){
		// Starting and ending indices for the node connectivity
		startI =m_mesh->m_element2NodeStart[elemi];
		endI =m_mesh->m_element2NodeStart[elemi+1];

		// Looping over the nodes of elementI
		for (int i=startI;i<endI;++i){
			nodeI = m_mesh->m_element2Node[i];
			m_mesh->m_node2ElementStart[nodeI+1] += 1;
		}
	}


	// Setting up the start offset linked list
	for (int i=1;i<m_mesh->m_nNode+1;++i){
		m_mesh->m_node2ElementStart[i] += m_mesh->m_node2ElementStart[i-1];
	}
}

void MeshGenerator::SolveNode2Element(){
	int startI;
	int endI;
	int nodeI;
	int j;

	// Initializing the node to element linked list connectivity
	m_mesh->m_node2Element = new int[m_mesh->m_element2NodeStart[m_mesh->m_nElement]]();
	
	// Array to save the increment
	int store[m_mesh->m_nNode]={0};

	for (int elemi=0;elemi<m_mesh->m_nElement;++elemi){
		startI=m_mesh->m_element2NodeStart[elemi];
		endI=m_mesh->m_element2NodeStart[elemi+1];

		for (int i=startI;i<endI;++i){
			nodeI=m_mesh->m_element2Node[i];
			// Fetching the index for the connectivity list
			j=m_mesh->m_node2ElementStart[nodeI]+store[nodeI];
			// Adding the element index for nodeI
			m_mesh->m_node2Element[j] = elemi;
			store[nodeI]+=1;
		}
	}
}

void MeshGenerator::SolveElement2ElementStart(){
	m_mesh->m_element2ElementStart = new int[m_mesh->m_nElement+1]();

	// Counting the number of faces per elements
	int nLocalFaces;
	int startI;
	int endI;

	for (int elemi = 0; elemi < m_mesh->m_nElement; ++elemi) {
		startI = m_mesh->m_element2NodeStart[elemi];
		endI = m_mesh->m_element2NodeStart[elemi + 1];
		nLocalFaces = endI - startI;
		m_mesh->m_element2ElementStart[elemi + 1] += nLocalFaces;
	}
	// Final setup of the element 2 element start offset linked list
	for (int i = 1; i < m_mesh->m_nElement + 1; ++i) {
		m_mesh->m_element2ElementStart[i] += m_mesh->m_element2ElementStart[i - 1];
	}
}

void MeshGenerator::SolveElement2FaceStart(){
	m_mesh->m_element2FaceStart = m_mesh->m_element2ElementStart;
}

void MeshGenerator::CountFaces(){
	int numFaceBC = m_mesh->m_markers->GetNElement();
	int sumTot = m_mesh->m_element2NodeStart[m_mesh->m_nElement];

	// A partir de sumTot et numFaceBC, il est possible de calculer le nombre de faces au total dans le maillage (sans double comptage)
	m_mesh->m_nFace = (sumTot + numFaceBC) / 2;	
}

void MeshGenerator::SolveFaceConnectivity(){
	int nLocalFaces;
	int startI;
	int endI;

	// Initializing the element 2 element linked list to -1 using fill_n from <algorithm>
	m_mesh->m_element2Element = new int[m_mesh->m_element2ElementStart[m_mesh->m_nElement]];
	std::fill_n(m_mesh->m_element2Element, m_mesh->m_element2ElementStart[m_mesh->m_nElement], -1);

	// Initializing the face surrounding element linked list to -1
	m_mesh->m_element2Face = new int[m_mesh->m_element2ElementStart[m_mesh->m_nElement]];
	std::fill_n(m_mesh->m_element2Face, m_mesh->m_element2ElementStart[m_mesh->m_nElement], -1);

	// Initializing the face2element linked list to -1
	m_mesh->m_face2Element = new int[m_mesh->m_nFace * 2];
	std::fill_n(m_mesh->m_face2Element, m_mesh->m_nFace * 2, -1);

	// Initializing the face2node linked list to -1
	// TODO Changer pour le 3D
	// On doit d'abord calculer la longueur de cette liste
	// La longueur de la liste correspond a la somme du nombre de noeuds par face
	// Dans l'exemple presente dans le manuel, le nombre de noeuds par face est 2 pour chacune des faces (le cas pour tous les maillages 2D)
	int numNodePerFace[m_mesh->m_nFace];// Idealement cet array est une sortie de MeshReader
	for (int i = 0; i < m_mesh->m_nFace; ++i) {
		numNodePerFace[i] = 2;
		m_longueurFace2node += numNodePerFace[i];
	}
	// Initializing the face2node linked list to
	m_mesh->m_face2Node =new int[m_longueurFace2node];

	// Array to save information to speedup the process
	m_lhelp = std::unique_ptr<int[]>(new int[m_nNodesForFace]());
	int lpoint[m_mesh->m_nNode] = {0};
	int ipoint;
	int elemStart;
	int elemEnd;
	int elemj;
	int startJ;
	int endJ;
	int nNodesForFaceI;
	int nNodesForFaceJ;
	int count;
	int pointIndex;
	int nLocaleFacesJ;
	int faceCount = 0;
	m_nodeCount = 0;

	// Looping over all elements in the mesh
	for (int elemi = 0; elemi < m_mesh->m_nElement; ++elemi) {
		startI = m_mesh->m_element2NodeStart[elemi];
		endI = m_mesh->m_element2NodeStart[elemi + 1];
		nLocalFaces = endI - startI;


		// Looping over the local faces of elemi
		for (int faceI = 0; faceI < nLocalFaces; ++faceI) {
			// In 2D, a face has 2 nodes
			nNodesForFaceI = m_nNodesForFace;// to be changed for 3D m_nNodesForFace to be an array

			//Saving the nodes of the face to find
			m_lhelp[0] = m_mesh->m_element2Node[startI + faceI];
			if (faceI == nLocalFaces - 1) {
				m_lhelp[1] = m_mesh->m_element2Node[startI];
			} else {
				m_lhelp[1] = m_mesh->m_element2Node[startI + faceI + 1];
			}

			// Storing the nodes of the face
			lpoint[m_lhelp[0]] = 1;
			lpoint[m_lhelp[1]] = 1;

			// Choosing the first node to loop over its Elements
			ipoint = m_lhelp[0];

			elemStart = m_mesh->m_node2ElementStart[ipoint];
			elemEnd = m_mesh->m_node2ElementStart[ipoint + 1];


			// Looping over the elements connected to point ipoint
			for (int j = elemStart; j < elemEnd; ++j) {
				elemj = m_mesh->m_node2Element[j];

				if (elemj != elemi) {
					startJ = m_mesh->m_element2NodeStart[elemj];
					endJ = m_mesh->m_element2NodeStart[elemj + 1];
					nLocaleFacesJ = endJ - startJ;

					// Looping over the faces of elementJ
					for (int facej = 0; facej < nLocaleFacesJ; ++facej) {
						// TODO Dont forget to change to an array in 3D! nNodesforFace[elemj]
						nNodesForFaceJ = 2;
						if (nNodesForFaceI == nNodesForFaceJ) {
							count = 0;
							for (int localNodeJ = 0; localNodeJ < nNodesForFaceJ; ++localNodeJ) {
								pointIndex = startJ + facej + localNodeJ;

								if (facej == (nLocaleFacesJ - 1) && localNodeJ == (nNodesForFaceJ - 1)) {
									pointIndex = startJ;
								}
								count += lpoint[m_mesh->m_element2Node[pointIndex]];
								// If the number of matching nodes is equal to the number of nodes in faceI, faceJ is a match with faceI
								if (count == nNodesForFaceI) {
									//Adding elementJ to the connectivity of elementI
									m_mesh->m_element2Element[m_mesh->m_element2ElementStart[elemi] + faceI] = elemj;

									// Checking if the face has already been added
									if (m_mesh->m_element2Element[m_mesh->m_element2ElementStart[elemj] + facej] == elemi) {
										m_mesh->m_element2Face[m_mesh->m_element2ElementStart[elemi] + faceI] = m_mesh->m_element2Face[m_mesh->m_element2ElementStart[elemj] + facej];
									}
									// Adding a new face
									else {
										m_mesh->m_element2Face[m_mesh->m_element2ElementStart[elemi] + faceI] = faceCount;
										m_mesh->m_element2Face[m_mesh->m_element2ElementStart[elemj] + facej] = faceCount;
										m_mesh->m_face2Element[2 * faceCount + 0] = elemi;
										m_mesh->m_face2Element[2 * faceCount + 1] = elemj;
										for (int i = 0; i < nNodesForFaceI; ++i) {
											m_mesh->m_face2Node[m_nodeCount] = m_lhelp[i];
											m_nodeCount += 1;
										}
										faceCount += 1;
									}
								}
							}
						}
					}
				}
			}

			for (int i = 0; i < nNodesForFaceI; ++i) {
				lpoint[m_lhelp[i]] = 0;
			}
		}
	}

	// Parcours de esuel pour calculer le nombre d'elements au total
	int elemCount = m_mesh->m_nElement;
	int nElemTot;
	int nGhostCells;

	for (int i = 0; i < m_mesh->m_element2ElementStart[m_mesh->m_nElement]; ++i) {
		if (m_mesh->m_element2Element[i] == -1) {
			m_mesh->m_element2Element[i] = elemCount;
			elemCount += 1;
		}
	}
	nElemTot = elemCount;
	nGhostCells = nElemTot - m_mesh->m_nElement;


	m_mesh->m_nFaceInt = faceCount;// Nombre de faces interne
	//int nBondFaces;           // Nombre de faces frontieres


	// Parcours de fsuel pour calculer le nombre de faces au total

	for (int i = 0; i < m_mesh->m_element2ElementStart[m_mesh->m_nElement]; ++i) {
		if (m_mesh->m_element2Face[i] == -1) {
			m_mesh->m_element2Face[i] = faceCount;
			faceCount += 1;
		}
	}
	//nBondFaces = m_mesh->m_nFace - m_mesh->m_nFaceInt;

}

void MeshGenerator::SolveFace2Element(){
	// Initializing the face2element connectivity
	int condition;
	int condition2;
	int condition3;
	int compteur;
	int i;
	int j;
	int k;
	int indiceFace[2] = {0};
	int elemi;
	int elemj;

	for (int faceI = m_mesh->m_nFaceInt; faceI < m_mesh->m_nFace; ++faceI) {//Looping over the faces
		compteur = 0;
		condition = 1;
		i = 0;
		while (condition) {
			if (m_mesh->m_element2Face[i] == faceI) {
				indiceFace[compteur] = i;
				compteur += 1;
			}
			if (compteur == 2) {
				condition = 0;
			} else if (compteur == 1 && i > m_mesh->m_element2FaceStart[m_mesh->m_nElement]) {
				indiceFace[compteur] = m_mesh->m_element2FaceStart[m_mesh->m_nElement];
				condition = 0;
			} else {
				i += 1;
			}
		}
		condition2 = 1;
		j = 0;
		while (condition2) {
			if (m_mesh->m_element2FaceStart[j] <= indiceFace[0] && m_mesh->m_element2FaceStart[j + 1] > indiceFace[0]) {
				elemi = j;
				condition2 = 0;
			} else {
				j += 1;
			}
		}
		condition3 = 1;
		k = 0;
		while (condition3) {
			if (m_mesh->m_element2FaceStart[k] <= indiceFace[1] && m_mesh->m_element2FaceStart[k + 1] > indiceFace[1]) {
				elemj = k;
				condition3 = 0;
			} else {
				k += 1;
			}

			if (k == m_mesh->m_nElement) {
				elemj = -1;
				condition3 = 0;
			}
		}
		m_mesh->m_face2Element[2 * faceI + 0] = elemi;
		m_mesh->m_face2Element[2 * faceI + 1] = elemj;
	}


	// Passage dans face2element pour mettre a jour les ghost cells
	int countGhostcells = m_mesh->m_nElement;
	for (int i = 0; i < 2 * m_mesh->m_nFace; ++i) {
		if (m_mesh->m_face2Element[i] == -1) {
			m_mesh->m_face2Element[i] = countGhostcells;
			countGhostcells += 1;
		}
	}

}

void MeshGenerator::SolveFace2Node(){
	// Passage dans face2node pour mettre a jour les ghost cells
	int countGhostcells = m_mesh->m_nElement;
	int trouve;
	int startI;
	int endI;
	int nLocalFaces;

	//int depasse=0;
	for (int elemi = 0; elemi < m_mesh->m_nElement; ++elemi) {
		startI = m_mesh->m_element2NodeStart[elemi];
		endI = m_mesh->m_element2NodeStart[elemi + 1];
		nLocalFaces = endI - startI;

		for (int faceI = 0; faceI < nLocalFaces; ++faceI) {// En 2D, cette ligne est vraie,
			                                               // en 3D, ce n'est pas vrai que le nombre de noeuds dun element correspond
			                                               //au nombre de faces, il faut donc penser a creer, en plus de numNodeElem,
			                                               // un vecteur qui va stocker le nombre de faces par element!!
			                                               // for (int faceI=0;faceI<numFaceElem[elemi])...
			m_lhelp[0] = m_mesh->m_element2Node[startI + faceI];
			if (faceI == nLocalFaces - 1) {
				m_lhelp[1] = m_mesh->m_element2Node[startI];
			} else {
				m_lhelp[1] = m_mesh->m_element2Node[startI + faceI + 1];
			}

			trouve = 0;
			for (int i = 0; i < m_longueurFace2node / 2; ++i) {
				if (m_mesh->m_face2Node[i * m_nNodesForFace] == m_lhelp[0] && m_mesh->m_face2Node[i * m_nNodesForFace + 1] == m_lhelp[1] || m_mesh->m_face2Node[i * m_nNodesForFace] == m_lhelp[1] && m_mesh->m_face2Node[i * m_nNodesForFace + 1] == m_lhelp[0]) {
					trouve = 1;
				}
			}

			if (!trouve) {
				m_mesh->m_face2Node[m_nodeCount] = m_lhelp[0];
				m_mesh->m_face2Node[m_nodeCount + 1] = m_lhelp[1];
				m_nodeCount += 2;
			}
		}
	}
}
