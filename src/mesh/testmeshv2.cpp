//1.-) Entête du programme
/////////////////////////////////////////////////////////////////////////////
//FICHIER: Mesh.cpp
//AUTEUR: Olivier Levesque et Abraham Bherer-Constant
//DATE: 24 septembre 2020
/*DESCRIPTION: Premiere version d'une fonction Mesh qui cree les tables de
connectivite et autres informations necessaires a la resolution
*/
/////////////////////////////////////////////////////////////////////////////

//2.-)/INCLUSION DE FICHIERS ET DE LIBRAIRIES
#include <iostream>
#include <string>
#include <limits.h>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	//3.-) DECLARATION DES VARIABLES
	//Variables obtenues par le MeshReader/MeshGenerator (dans l'ordre d'apparition selon format SU2)
	int NDIME; 			//Nombre de dimensions
	int NPOIN=13; 		//Nombre de points (noeuds) du maillage
	double COORD; 		//Table des coordonnees de chaque point du maillage (x,y,z)
	int NELEM=13; 		//Nombre d'elements dans le maillage
	int NNODE=3;		//Nombre de noeuds par element, ATTENTION!! peut varier pour un maillage non structure. il est preferable de produire une table avec NNODE pour chaque element!
	int CONNEC[NELEM][NNODE]; //Table des connectivites entre les elements et les noeuds. La premiere colonne de chaque ligne decrit le type d'element (ligne, triangle, carre,etc.)
	int NMARK; 			//Nombre de TAGS au bord du domaine
	string MARKER_TAG; 	//Chaine de caractere decrivant le TAG du bord (ex: upper,lower,rigt,left) Il nous faut une convention pour l'ordre d'apparition!!
	int MARKER_ELEMS; 	//Table signifiant le nombre d'elements de bord pour chaque TAG (ex: le premier TAG est lower, et il y a 3 elements dans le bas du domaine --> le premier element de MARKER_ELEMS=3)
	int CONNECB; 		//Table des connectivites entres les noeuds pour les elements du bord du domaine. La premiere colonne de chaque ligne decrit le type d'element (ligne, triangle, carre,etc.)
	//
	//
	// Variables a generer avec la fonction Mesh
	int nunke=5; 				//Nombre d'inconnues par element
	double unkne[NELEM][nunke];	//Matrice des inconnues en fonction de l'element
	//Ajouter les conditions frontieres!!


	//Variables utiles pour la generation de esup1,esup2,etc.
	int ipoil;					//Variable utilisee pour creer esup1,esup2
	int ipoin;
	int istor;


	//test, initiation de CONNEC
	CONNEC[0][0]=0;
	CONNEC[0][1]=1;
	CONNEC[0][2]=5;
	CONNEC[1][0]=1;
	CONNEC[1][1]=6;
	CONNEC[1][2]=5;
	CONNEC[2][0]=1;
	CONNEC[2][1]=2;
	CONNEC[2][2]=6;
	CONNEC[3][0]=2;
	CONNEC[3][1]=7;
	CONNEC[3][2]=6;
	CONNEC[4][0]=2;
	CONNEC[4][1]=3;
	CONNEC[4][2]=7;
	CONNEC[5][0]=3;
	CONNEC[5][1]=8;
	CONNEC[5][2]=7;
	CONNEC[6][0]=3;
	CONNEC[6][1]=4;
	CONNEC[6][2]=8;
	CONNEC[7][0]=4;
	CONNEC[7][1]=9;
	CONNEC[7][2]=8;
	CONNEC[8][0]=6;
	CONNEC[8][1]=7;
	CONNEC[8][2]=12;
	CONNEC[9][0]=7;
	CONNEC[9][1]=11;
	CONNEC[9][2]=12;
	CONNEC[10][0]=7;
	CONNEC[10][1]=8;
	CONNEC[10][2]=11;
	CONNEC[11][0]=8;
	CONNEC[11][1]=10;
	CONNEC[11][2]=11;
	CONNEC[12][0]=8;
	CONNEC[12][1]=9;
	CONNEC[12][2]=10;

	cout << "La table CONNEC de l'exemple est: " << endl;

	for (int i=0;i<NELEM;++i){
		for (int j=0;j<3;++j){
			printf("%2d ",CONNEC[i][j]);
		}
		printf("\n");
	}


	//4.-) Debut du code
	//________________________________________________________ Cell to node connectivity__________________________________________
	printf("______________________Debut code cell2node________________________\n\n" );
	//Ici, ca va nous prendre un vecteur qui aura store le nombre de noeuds par element a la lecture du fichier de maillage. Nommons ce vecteur numNodeElem
	//Pour l'exemple present, numNodeElem un est vecteur de longueur 13x1 qui possede des "3" a chaque entree
	//Le but est de sommer ce vecteur pour avoir la dimension exacte de cell2node et node2cell (esup)
	int numNodeElem[NPOIN]={0};
	int sumTot=0;

	for (int i=0;i<NPOIN;++i){
		numNodeElem[i]=3;
	}

	//Calcul de sumTot (j'ai separe le calcul de sumTot pcq normalement numNodeElem doit etre une sortie de la lecture du maillage)
	for(int i=0;i<NPOIN;++i){
		sumTot+=numNodeElem[i];
	}

	int cell2nodeStart[NPOIN+1]={0};
	int cell2node[sumTot]={0};


	for (int i=0;i<NELEM;++i){
		cell2nodeStart[i+1]=cell2nodeStart[i]+numNodeElem[i];
		for (int j=0;j<numNodeElem[i];++j){
			cell2node[cell2nodeStart[i]+j]=CONNEC[i][j];
		}
	}
	int numNodes = 0;
	for (int i = 0; i < cell2nodeStart[NPOIN]; ++i) {
		if (cell2node[i] + 1 > numNodes) {
			numNodes = cell2node[i] + 1;
		}
	}
	printf("numNodes = %2d\n", numNodes);

	printf("Les linked lists pour la connectivite Elements a Noeuds sont les suivantes\n\n");
	printf("cell2nodeStart = \n");
	for (int i=0;i<NPOIN+1;++i){
		printf("%2d ",cell2nodeStart[i]);
	}
	printf("\n\n");
	printf("cell2node = \n");
	for (int i=0;i<sumTot;++i){
		printf("%2d ",cell2node[i]);
	}
	printf("\n\n");

	//________________________________________________________ Elements surrounding points__________________________________________
	printf("______________________Debut code ESUP________________________\n\n" );
	int esupStart[NPOIN+1]={0};
	int numEleSurPoi[NPOIN]={0};				//Vecteur indiquant le nombre d'elements qui entourent un point(noeud)
	int startI;
	int endI;
	int nodeI;
	int j;


	// Looping over the elements to count the number of elements around each nodes
	for (int elemi=0;elemi<NELEM;++elemi){
		// Starting and ending indices for the node connectivity
		startI=cell2nodeStart[elemi];
		endI=cell2nodeStart[elemi+1];

		// Looping over the nodes of elementI
		for (int i=startI;i<endI;++i){
			nodeI=cell2node[i];
			esupStart[nodeI+1] += 1;
		}
	}

	// Creating the array that stores the amount of Elements surrounding a Point
	for (int i=0;i<NPOIN;++i){
		numEleSurPoi[i]=esupStart[i+1];
	}

	// Setting up the start offset linked list
	for (int i=1;i<NPOIN+1;++i){
		esupStart[i] += esupStart[i-1];
	}

	// Initializing the node to element linked list connectivity
	int esup[sumTot]={0};
	// Array to save the increment
	int store[NPOIN]={0};

	for (int elemi=0;elemi<NELEM;++elemi){
		startI=cell2nodeStart[elemi];
		endI=cell2nodeStart[elemi+1];

		for (int i=startI;i<endI;++i){
			nodeI=cell2node[i];
			// Fetching the index for the connectivity list
			j=esupStart[nodeI]+store[nodeI];
			// Adding the element index for nodeI
			esup[j] = elemi;
			store[nodeI]+=1;
		}
	}

	printf("Les linked lists pour la connectivite Noeuds a Elements sont les suivantes\n\n");
	printf("esupStart = \n");
	for (int i=0;i<NPOIN+1;++i){
		printf("%2d ",esupStart[i]);
	}
	printf("\n\n");
	printf("esup = \n");
	for (int i=0;i<sumTot;++i){
		printf("%2d ",esup[i]);
	}
	printf("\n\n");

	//Test element
	int indice=8;
	cout << "Les elements qui entourent le point " << indice << " debutent par l'indice " << esupStart[indice] << " et terminent par l'indice " << esupStart[indice+1]-1 << " dans le vecteur esup1." << endl;
	cout << "Ces elements sont les elements: " ;
	for (int i=esupStart[indice];i<esupStart[indice+1];++i){
		printf("%2d ", esup[i]);
	}
	printf("\n\n");


	//___________________________________________ Elements surrounding Elements____________________________________________
	printf("_________________Debut code ESUEL+FSUEL________________________\n\n");
	// Element 2 element start offset linked list
	int esuelStart[NELEM+1]={0};
	int fsuelStart[NELEM + 1] = {0};

	// Counting the number of faces per elements
	int nLocalFaces;

	for (int elemi=0;elemi<NELEM;++elemi){
		startI=cell2nodeStart[elemi];
		endI=cell2nodeStart[elemi+1];
		nLocalFaces=endI-startI;
		esuelStart[elemi+1]+=nLocalFaces;
	}

	// Final setup of the element 2 element start offset linked list
	for (int i=1;i<NELEM+1;++i){
		esuelStart[i]+=esuelStart[i-1];
	}
	for (int i = 0; i < NELEM + 1; ++i) {
		fsuelStart[i] = esuelStart[i];
	}

	// Initializing the element 2 element linked list to -1 using fill_n from <algorithm>
	int esuel[esuelStart[NELEM]];
	std::fill_n(esuel,esuelStart[NELEM],-1);
	// Initializing the face surrounding element linked list to -1
	int fsuel[esuelStart[NELEM]];
	std::fill_n(fsuel, esuelStart[NELEM], -1);

	// Array to save information to speedup the process
	int nNodesForFace=2; // In 2D a face necesseraly has 2 nodes. this needs to be changed for a 3D mesh
	int lhelp[nNodesForFace]={0};
	int lpoint[NPOIN]={0};
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

	std::vector<int> face2node;

	// Looping over all elements in the mesh
	for (int elemi=0;elemi<NELEM;++elemi)
	{
		startI = cell2nodeStart[elemi];
		endI = cell2nodeStart[elemi+1];
		nLocalFaces = endI - startI;


		// Looping over the local faces of elemi
		for (int faceI=0;faceI<nLocalFaces;++faceI){
			// In 2D, a face has 2 nodes
			nNodesForFaceI = nNodesForFace;// to be changed for 3D nNodesForFace to be an array

			//Saving the nodes of the face to find
			lhelp[0] = cell2node[startI+faceI];
			if (faceI == nLocalFaces-1){
				lhelp[1] = cell2node[startI];
			}
			else {
				lhelp[1] = cell2node[startI+faceI+1];
			}

			// Storing the nodes of the face
			lpoint[lhelp[0]] = 1;
			lpoint[lhelp[1]] = 1;

			// Choosing the first node to loop over its Elements
			ipoint = lhelp[0];

			elemStart = esupStart[ipoint];
			elemEnd = esupStart[ipoint+1];


			// Looping over the elements connected to point ipoint
			for (int j=elemStart;j<elemEnd;++j){
				elemj = esup[j];

				if (elemj != elemi){
					startJ = cell2nodeStart[elemj];
					endJ = cell2nodeStart[elemj+1];
					nLocaleFacesJ = endJ - startJ;

					// Looping over the faces of elementJ
					for (int facej=0;facej<nLocaleFacesJ;++facej){
						nNodesForFaceJ = 2;// Dont forget to change to an array in 3D! nNodesforFace[elemj]

						if (nNodesForFaceI == nNodesForFaceJ){
							count = 0;
							for (int localNodeJ=0;localNodeJ<nNodesForFaceJ;++localNodeJ){
								pointIndex = startJ+facej+localNodeJ;

								if (facej == (nLocaleFacesJ-1) && localNodeJ == (nNodesForFaceJ-1)){
									pointIndex = startJ;

								}
								count += lpoint[cell2node[pointIndex]];
							// If the number of matching nodes is equal to the number of nodes in faceI, faceJ is a match with faceI
							if (count == nNodesForFaceI){
								//Adding elementJ to the connectivity of elementI
								esuel[esuelStart[elemi]+faceI]=elemj;

								// Checking if the face has already been added
								if (esuel[esuelStart[elemj] + facej] == elemi) {
									fsuel[esuelStart[elemi] + faceI] = fsuel[esuelStart[elemj] + facej];
								}
								// Adding a new face
								else {
									fsuel[esuelStart[elemi] + faceI] = faceCount;
									fsuel[esuelStart[elemj] + facej] = faceCount;
									for (int i = 0; i < nNodesForFaceI; ++i) {
										face2node.push_back(lhelp[i]);
									}
									faceCount += 1;
								}
							}

							}


						}
					}
				}
			}

			for (int i=0;i<nNodesForFaceI;++i){
				lpoint[lhelp[i]]=0;
			}
		}
	}

	printf("face2node = \n");
	for (int i = 0; i < face2node.size(); ++i) {
		printf("%2d ", face2node[i]);
	}
	printf("\n\n");

	// Parcours de esuel pour calculer le nombre d'elements au total
	int elemCount = NELEM;
	int nElemTot;
	int nGhostCells;

	for (int i = 0; i < esuelStart[NELEM]; ++i) {
		if (esuel[i] == -1) {
			esuel[i] = elemCount;
			elemCount += 1;
		}
	}
	nElemTot = elemCount;
	nGhostCells = nElemTot - NELEM;


	int nIntFaces = faceCount;// Nombre de faces interne
	int nFaces;               // Nombre total de nFaces
	int nBondFaces;           // Nombre de faces frontieres


	// Parcours de fsuel pour calculer le nombre de faces au total

	for (int i = 0; i < esuelStart[NELEM]; ++i) {
		if (fsuel[i] == -1) {
			fsuel[i] = faceCount;
			faceCount += 1;
		}
	}

	nFaces = faceCount;
	nBondFaces = nFaces - nIntFaces;

	// Initializing the face2element connectivity
	int face2el[2 * nFaces];
	std::fill_n(face2el, 2 * nFaces, -1);

	int condition;
	int condition2;
	int condition3;
	int compteur;
	int i;
	int k;
	int indiceFace[2] = {0};
	int elemi;

	for (int faceI = 0; faceI < nFaces; ++faceI) {//Looping over the faces
		compteur = 0;
		condition = 1;
		i = 0;
		while (condition) {
			if (fsuel[i] == faceI) {
				indiceFace[compteur] = i;
				compteur += 1;
			}
			if (compteur == 2) {
				condition = 0;
			} else if (compteur == 1 && i > fsuelStart[NELEM]) {
				indiceFace[1] = fsuelStart[NELEM];
				condition = 0;
			} else {
				i += 1;
			}
		}
		condition2 = 1;
		j = 0;
		while (condition2) {
			if (fsuelStart[j] <= indiceFace[0] && fsuelStart[j + 1] > indiceFace[0]) {
				elemi = j;
				condition2 = 0;
			} else {
				j += 1;
			}
		}
		condition3 = 1;
		k = 0;
		while (condition3) {
			if (fsuelStart[k] <= indiceFace[1] && fsuelStart[k + 1] > indiceFace[1]) {
				elemj = k;
				condition3 = 0;
			} else {
				k += 1;
			}

			if (k == NELEM) {
				elemj = -1;
				condition3 = 0;
			}
		}
		face2el[2 * faceI + 0] = elemi;
		face2el[2 * faceI + 1] = elemj;
	}

	// Passage dans face2el pour mettre a jour les ghost cells
	int countGhostcells = NELEM;
	for (int i = 0; i < 2 * nFaces; ++i) {
		if (face2el[i] == -1) {
			face2el[i] = countGhostcells;
			countGhostcells += 1;
		}
	}

	// //___________________________________________________________________________
	// // Debut code face2node
	// int lnode[numNodes]={0};
	//
	// for (int faceI=0;faceI<nIntFaces;++faceI){
	// 	elemi=face2el[2*faceI+0];
	// 	elemj=face2el[2*faceI+1];
	//
	//
	// 	for (int nodeI=0;nodeI<nNodesForFace;++nodeI){
	//
	// 	}
	//
	//
	// }


	//___________________________________________________________________________
	// Affichage des linked lists

	printf("esuelStart = \n");
	for (int i=0;i<NELEM+1;++i){
		printf("%2d ",esuelStart[i] );
	}
	printf("\n\n");

	printf("esuel = \n");
	for (int i=0;i<esuelStart[NELEM];++i){
		printf("%2d ",esuel[i] );
	}
	printf("\n\n");

	printf("Nombre d'elements internes = %2d\n", NELEM);
	printf("Nombre d'elements total = %2d\n", nElemTot);
	printf("Nombre de cellules fantomes = %2d\n", nGhostCells);
	printf("\n\n");

	printf("fsuelStart = \n");
	for (int i = 0; i < NELEM + 1; ++i) {
		printf("%2d ", fsuelStart[i]);
	}
	printf("\n\n");

	printf("fsuel = \n");
	for (int i = 0; i < esuelStart[NELEM]; ++i) {
		printf("%2d ", fsuel[i]);
	}
	printf("\n\n");

	printf("Nombre de faces internes = %2d\n", nIntFaces);
	printf("Nombre de faces  = %2d\n", nFaces);
	printf("Nombre de faces frontieres = %2d\n", nBondFaces);
	printf("\n\n");

	printf("face2el = \n");
	for (int i = 0; i < 2 * nFaces; ++i) {
		printf("%2d ", face2el[i]);
	}
	printf("\n\n");

	printf("face2node = \n");
	for (int i = 0; i < 2 * nFaces; ++i) {
		printf("%2d ", face2node[i]);
	}
	printf("\n\n");
}
