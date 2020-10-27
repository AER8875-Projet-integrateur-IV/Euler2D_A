#include "../../../src/mesh/generator/MeshGenerator.hpp"
#include "../../../src/mesh/MarkerContainer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

// ----------- MeshGenerator2SquareTEST----------------

// class MeshGenerator2SquareTEST : public ::testing::Test {
//     // Example taken from:
// protected:
//     WrapperMeshGenerator generator;
//     Mesh mesh;

//     void SetUp() override {
//         mesh.m_nNode = 6;
//         mesh.m_nDime = 2;
//         mesh.m_nElement =2;

//         generator.SetnNode(6);
//         generator.SetnDime(2);
//         generator.SetnElement(2);

//         int element2Node_val[8] = {0,3,4,1,1,4,5,2};
//         int* element2Node = new int[8];
//         for (int i=0;i<8;i++){
//             element2Node[i] = element2Node_val[i]; 
//         }

//         int element2NodeStart_val[3] = {0,4,8};
//         int* element2NodeStart = new int[3];
//         for (int i=0;i<3;i++){
//             element2NodeStart[i] = element2NodeStart_val[i]; 
//         }        
        
//         generator.Setelement2Node(element2Node);
//         generator.Setelement2NodeStart(std::unique_ptr<int[]>(element2NodeStart));
// 	}

// };

// TEST_F(MeshGenerator2SquareTEST, SolveNode2ElementStart) {
//     generator.CallSolveNode2ElementStart();
// 	const double ar_exp[] = {0,1,3,4,5,7,8};

//     int* ar_ptr = (*(generator.Getnode2ElementStart())).get();
// 	std::vector<int> ar(ar_ptr, ar_ptr + 7);
//     ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
// }

// TEST_F(MeshGenerator2SquareTEST, SolveNode2Element) {
//     generator.CallSolveNode2ElementStart();
//     generator.CallSolveNode2Element();
// 	const double ar_exp[] = {0,0,1,1,0,0,1,1};

//     int* ar_ptr = (*(generator.Getnode2Element())).get();
// 	std::vector<int> ar(ar_ptr, ar_ptr + 8);
// 	ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
// }



// ----------- MeshGeneratorManualTEST----------------
class MeshGeneratorManualTEST : public ::testing::Test {
    // Example taken from:
    // Applied Computational Fluid Dynamics Techniques: An Introduction Based on Finite Element Methods, Second Edition.
    // Rainald Löhner © 2008 John Wiley & Sons, Ltd. ISBN: 978-0-470-51907-3
protected:
    MeshGenerator* generator;
    Mesh mesh;
    MeshGeneratorManualTEST(){
        mesh = Mesh();
        mesh.m_nDime = 2;
        mesh.m_nElement = 13;
        mesh.m_nNode = 13;

        int element2Node_val[39] = {0,1,5,1,6,5,1,2,6,2,7,6,2,3,7,3,8,7,3,4,8,4,9,8,6,7,12,7,11,12,7,8,11,8,10,11,8,9,10};
        int* element2Node = new int[39];
        for (int i=0;i<39;i++){
            element2Node[i] = element2Node_val[i]; 
        }
        mesh.m_element2Node = element2Node;

        int element2NodeStart_val[14] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39};
        int* element2NodeStart = new int[14];
        for (int i=0;i<14;i++){
            element2NodeStart[i] = element2NodeStart_val[i]; 
        }        
        mesh.m_element2NodeStart = element2NodeStart;
        
        mesh.m_markers = new MarkerContainer(1);
        std::string tag = "testtag";
        int nElement = 11;
        int* marker_element2Node = new int[1];
        int* marker_element2NodeStart = new int[1];
        int* marker_elemenet2VTKId = new int[1];
        mesh.m_markers->AddMarker(&tag,&nElement,marker_element2Node,marker_element2NodeStart,marker_elemenet2VTKId);

        generator = new MeshGenerator(&mesh);
    }
    ~MeshGeneratorManualTEST(){
        delete generator;
    }
};

TEST_F(MeshGeneratorManualTEST, SolveNode2ElementStart) {
    generator->SolveNode2ElementStart();
	const double ar_exp[] = {0,1,4,7,10,12,14,18,24,30,32,34,37,39};

    int* ar_ptr = mesh.m_node2ElementStart;
	std::vector<int> ar(ar_ptr, ar_ptr + 14);
    ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
}

TEST_F(MeshGeneratorManualTEST, SolveNode2Element) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();

	const double ar_exp[] = {0,0,1,2,2,3,4,4,5,6,6,7,0,1,1,2,3,8,3,4,5,8,9,10,5,6,7,10,11,12,7,12,11,12,9,10,11,8,9};

    int* ar_ptr = mesh.m_node2Element;
	std::vector<int> ar(ar_ptr, ar_ptr + 39);
	ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
}

TEST_F(MeshGeneratorManualTEST, SolveElement2ElementStart) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();
    generator->SolveElement2ElementStart();
	const double ar_exp[] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39};

    int* ar_ptr = mesh.m_element2ElementStart;
	std::vector<int> ar(ar_ptr, ar_ptr + 14);
	ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
}

TEST_F(MeshGeneratorManualTEST, Solveelement2FaceStart) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();
    generator->SolveElement2ElementStart();
    generator->SolveElement2FaceStart();
	const double ar_exp[] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39};

    int* ar_ptr = mesh.m_element2FaceStart;
	std::vector<int> ar(ar_ptr, ar_ptr + 14);
	ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
}

TEST_F(MeshGeneratorManualTEST, CountFaces) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();
    generator->SolveElement2ElementStart();
    generator->SolveElement2FaceStart();
    generator->CountFaces();
    int result = mesh.m_nFace;
	ASSERT_EQ(result, 25);    
}

TEST_F(MeshGeneratorManualTEST, SolveFaceConnectivity) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();
    generator->SolveElement2ElementStart();
    generator->SolveElement2FaceStart();
    generator->CountFaces();
    generator->SolveFaceConnectivity();

    // Connectivity element2element array
	const double arElement2Element_exp[] = {13,1,14,2,15,0,16,3,1,4,8,2,17,5,3,6,10,4,18,7,5,19,12,6,3,9,20,10,21,8,5,11,9,12,22,10,7,23,11};
    int* ar_ptr = mesh.m_element2Element;
	std::vector<int> arElement2Element(ar_ptr, ar_ptr + 39);
	ASSERT_THAT(arElement2Element, testing::ElementsAreArray(arElement2Element_exp)); 

    // Connectivity element2face array
	const double arElement2Face_exp[] = {14,0,15,1,16,0,17,2,1,3,4,2,18,5,3,6,7,5,19,8,6,20,9,8,4,10,21,11,22,10,7,12,11,13,23,12,9,24,13};
    ar_ptr = mesh.m_element2Face;
	std::vector<int> arElement2Face(ar_ptr, ar_ptr + 39);
	ASSERT_THAT(arElement2Face, testing::ElementsAreArray(arElement2Face_exp));    

}

TEST_F(MeshGeneratorManualTEST, SolveFace2Element) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();
    generator->SolveElement2ElementStart();
    generator->SolveElement2FaceStart();
    generator->CountFaces();
    generator->SolveFaceConnectivity();
    generator->SolveFace2Element();
	const double ar_exp[] = {0,1,1,2,2,3,3,4,3,8,4,5,5,6,5,10,6,7,7,12,8,9,9,10,10,11,11,12,0,13,0,14,1,15,2,16,4,17,6,18,7,19,8,20,9,21,11,22,12,23};

    int* ar_ptr = mesh.m_face2Element;
	std::vector<int> ar(ar_ptr, ar_ptr + 50);
	ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
}

TEST_F(MeshGeneratorManualTEST, SolveFace2Node) {
    generator->SolveNode2ElementStart();
    generator->SolveNode2Element();
    generator->SolveElement2ElementStart();
    generator->SolveElement2FaceStart();
    generator->CountFaces();
    generator->SolveFaceConnectivity();
    generator->SolveFace2Element();
    generator->SolveFace2Node();
	const double ar_exp[] = {1,5,1,6,2,6,2,7,7,6,3,7,3,8,8,7,4,8,9,8,7,12,7,11,8,11,8,10,0,1,5,0,6,5,1,2,2,3,3,4,4,9,12,6,11,12,10,11,9,10};

    int* ar_ptr = mesh.m_face2Node;
	std::vector<int> ar(ar_ptr, ar_ptr + 50);
	ASSERT_THAT(ar, testing::ElementsAreArray(ar_exp));    
}