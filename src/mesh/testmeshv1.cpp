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
	int esup1[NELEM*NNODE]={0};		//Vecteur des elements qui entourent un point "elements surrounding point"
	int esup2[NPOIN+1]={0};			//Vecteur de dimension connue qui sert a obtenir les locations dans le vecteur esup1
	int nesup[NPOIN]={0};			//Vecteur indiquant le nombre d'elements qui entourent un point(noeud)

	//Variables utiles pour la generation de esup1,esup2,etc.
	int ipoil;					//Variable utilisee pour creer esup1,esup2
	int ipoin;
	int istor;


	//test, initiation de CONNEC
	CONNEC[0][0]=1;
	CONNEC[0][1]=2;
	CONNEC[0][2]=6;
	CONNEC[1][0]=2;
	CONNEC[1][1]=7;
	CONNEC[1][2]=6;
	CONNEC[2][0]=2;
	CONNEC[2][1]=3;
	CONNEC[2][2]=7;
	CONNEC[3][0]=3;
	CONNEC[3][1]=8;
	CONNEC[3][2]=7;
	CONNEC[4][0]=3;
	CONNEC[4][1]=4;
	CONNEC[4][2]=8;
	CONNEC[5][0]=4;
	CONNEC[5][1]=9;
	CONNEC[5][2]=8;
	CONNEC[6][0]=4;
	CONNEC[6][1]=5;
	CONNEC[6][2]=9;
	CONNEC[7][0]=5;
	CONNEC[7][1]=10;
	CONNEC[7][2]=9;
	CONNEC[8][0]=7;
	CONNEC[8][1]=8;
	CONNEC[8][2]=13;
	CONNEC[9][0]=8;
	CONNEC[9][1]=12;
	CONNEC[9][2]=13;
	CONNEC[10][0]=8;
	CONNEC[10][1]=9;
	CONNEC[10][2]=12;
	CONNEC[11][0]=9;
	CONNEC[11][1]=11;
	CONNEC[11][2]=12;
	CONNEC[12][0]=9;
	CONNEC[12][1]=10;
	CONNEC[12][2]=11;

	cout << "La table CONNEC de l'exemple est: " << endl;

	for (int i=0;i<NELEM;++i){
		for (int j=0;j<3;++j){
			printf("%2d ",CONNEC[i][j]);
		}
		printf("\n");
	}


	//4.-) Debut du code

	//________________________________________________________ Elements surrounding points__________________________________________
	printf("______________________Debut code ESUP________________________\n\n" );
	//Generer esup1 et esup2
	//Element Pass 1
	for (int i=0;i<NELEM;++i){
		for (int j=0;j<NNODE;++j){
			ipoil=CONNEC[i][j];
			esup2[ipoil]=esup2[ipoil]+1;
		}
	}

	for (int i=0;i<NPOIN;++i){
		nesup[i]=esup2[i+1];
	}
	//Storage/reshuffling Pass 1
	for (int i=1;i<NPOIN+2;++i){
		esup2[i]=esup2[i]+esup2[i-1];
	}

	//Element Pass 2: Store the elements in esup1
	for (int i=0; i<NELEM; ++i){
		for (int j=0; j<NNODE; ++j){
			ipoin=CONNEC[i][j]-1;
			istor=esup2[ipoin]+1;
			esup2[ipoin]=istor;
			esup1[istor-1]=i+1;
		}
	}
	//Storage/reshuffling Pass 1
	for (int i=NPOIN+1;i>0;--i){
		esup2[i]=esup2[i-1];
	}
	esup2[0]=0;


	//Affichage des vecteurs crees
	cout << "esup1=";
	for (int i=0;i<NELEM*NNODE;++i){
		printf("%2d ",esup1[i] );
	}
	printf("\n\n");
	cout << "esup2=";
	for (int i=0;i<NPOIN+1;++i){
		printf("%2d ",esup2[i] );
	}
	printf("\n\n");
	cout << "nesup=";
	for (int i=0;i<NPOIN;++i){
		printf("%2d ",nesup[i] );
	}
	printf("\n\n");

	//Test element 5
	int indice=5;
	cout << "Les elements qui entourent le point " << indice << " debutent par l'indice " << esup2[indice-1] << " et terminent par l'indice " << esup2[indice]-1 << " dans le vecteur esup1." << endl;
	cout << "Ces elements sont les elements: " << esup1[esup2[indice-1]] << ", " << esup1[esup2[indice]-1] << endl << endl;

	//Test element 9
	indice=9;
	cout << "Les elements qui entourent le point " << indice << " debutent par l'indice " << esup2[indice-1] << " et terminent par l'indice " << esup2[indice]-1 << " dans le vecteur esup1." << endl;
	cout << "Ces elements sont les elements: " ;
	for (int i=esup2[indice-1];i<esup2[indice];++i){
		printf("%2d ", esup1[i]);
	}
	printf("\n\n");

	//
	//__________________________________________Points surrounding points ________________________________________________________
	printf("______________________Debut code PSUP_______________________________________\n\n");
	std::vector<int> psup1;
	int psup2[NPOIN+1]={0};
	int npsup[NPOIN]={0};
	int lpoin[NPOIN]={0};
	istor=0;
	int ielem=0;
	int jpoin=0;


	//Creation de psup1 et psup2
	for(int ipoin=0;ipoin<NPOIN;++ipoin){
		for(int iesup=esup2[ipoin];iesup<esup2[ipoin+1];++iesup){
			ielem=esup1[iesup]-1;
			for (int inode=0; inode<NNODE;++inode){
				jpoin=CONNEC[ielem][inode];
				if (jpoin != (ipoin+1) && lpoin[jpoin-1]!=(ipoin+1)){
					++istor;
					psup1.push_back (jpoin);
					lpoin[jpoin-1]=ipoin+1;
				}
			}
		}
		psup2[ipoin+1]=istor;
	}

	//Creation de npsup
	for (int i=0;i<NPOIN;++i){
		npsup[i]=psup2[i+1]-psup2[i];
	}


	cout << "psup1=";
	for (int i=0;i<psup1.size();++i){
		printf("%2d ",psup1[i] );
	}
	printf("\n\n");
	cout << "psup2=";
	for (int i=0;i<NPOIN+1;++i){
		printf("%2d ",psup2[i] );
	}
	printf("\n\n");
	cout << "npsup=";
	for (int i=0;i<NPOIN;++i){
		printf("%2d ",npsup[i] );
	}
	printf("\n\n");
	cout << "lpoin=";
	for (int i=0;i<NPOIN;++i){
		printf("%2d ",lpoin[i] );
	}
	printf("\n\n");

	//Test point 5
	indice=5;
	cout << "Les points qui entourent le point " << indice << " debutent par l'indice " << psup2[indice-1] << " et terminent par l'indice " << psup2[indice]-1 << " dans le vecteur psup1." << endl;
	cout << "Ces points sont les points: " ;
	for (int i=psup2[indice-1];i<psup2[indice];++i){
		printf("%2d ", psup1[i]);
	}
	printf("\n\n");


	//Test point 9
	indice=9;
	cout << "Les points qui entourent le point " << indice << " debutent par l'indice " << psup2[indice-1] << " et terminent par l'indice " << psup2[indice]-1 << " dans le vecteur psup1." << endl;
	cout << "Ces points sont les points: " ;
	for (int i=psup2[indice-1];i<psup2[indice];++i){
		printf("%2d ", psup1[i]);
	}
	printf("\n\n");

	//___________________________________________ Elements surrounding Elements____________________________________________
	printf("_________________Debut code ESUEL________________________\n\n" );
	for (int i=0;i<NPOIN;++i){
		lpoin[i]=0;
	}
	int NFAEL=3;  //Cas particulier, dans notre code, il faudrait un vecteur avec le nombre de faces de chaque element
	int esuel[NELEM][NFAEL]={0};
	esuel[0][2]=0 ;//weird cette valeur donnait -06785434??

	//Declaration des variables utiles pour la creation de esuel
	int nnofa=2;
	int lhelp[nnofa]={0};
	int jelem;
	int nnofj;
	int icoun;

	//Creation de lpofa une matrice (ou liste) des points pour chaque face, specifique au type d'element, independant au maillage --> peut etre cree d'avance
	int lpofa[NFAEL][nnofa]={0};

	lpofa[0][0]=1;
	lpofa[0][1]=2;
	lpofa[1][0]=2;
	lpofa[1][1]=3;
	lpofa[2][0]=3;
	lpofa[2][1]=1;

	//Creation de lnofa
	int lnofa[NFAEL]={nnofa,nnofa,nnofa};

	// Code ESUEL________________________


	for (int ielem=0;ielem<NELEM;++ielem){
		for(int ifael=0;ifael<NFAEL;++ifael){
			nnofa=lnofa[ifael];
			for (int i=0;i<nnofa;++i){
				lhelp[i]=CONNEC[ielem][lpofa[ifael][i]-1];
			}
			for (int i=0;i<nnofa;++i){
				lpoin[lhelp[i]-1]=1;
			}
			ipoin=lhelp[0];
			for (int istor=esup2[ipoin-1];istor<esup2[ipoin]-1;++istor){
				jelem=esup1[istor];
				if (jelem-1 != ielem){
					for (int jfael=0;jfael<NFAEL;++jfael){
						nnofj=lnofa[jfael];
						if (nnofj == nnofa){
							icoun=0;
							for (int jnofa=0;jnofa<nnofa;++jnofa){
								jpoin=CONNEC[jelem-1][lpofa[jfael][jnofa]-1];
								icoun+=lpoin[jpoin-1];
							}
							if (icoun == nnofa){
								esuel[ielem][ifael]=jelem;
							}
						}
					}
				}
			}
			for (int i=0;i<nnofa;++i){
				lpoin[lhelp[i]-1]=0;
			}
		}
	}

	cout << "La table esuel de l'exemple est: " << endl;

	for (int i=0;i<NELEM;++i){
		for (int j=0;j<NFAEL;++j){
			printf("%2d ",esuel[i][j]);
		}
		printf("\n");
	}









}
