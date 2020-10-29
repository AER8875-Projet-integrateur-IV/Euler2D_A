#include "../../../src/mesh/Marker.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <memory>

class MarkerTEST : public ::testing::Test {
protected:
    Marker marker;

	void SetUp() override {
        std::string tag = "testMarker";
        int nElement = 2;

        int element2NodeStart_val[3] = {0,2,4};
        int* element2NodeStart = new int[3];
        for (int i=0;i<3;i++){
            element2NodeStart[i] = element2NodeStart_val[i]; 
        }      

        int element2Node_val[4] = {1,2,3,4};
        int* element2Node = new int[4];
        for (int i=0;i<4;i++){
            element2Node[i] = element2Node_val[i]; 
        }    

        int* element2VTKId = new int[1];

        marker.Set(&tag,&nElement,element2Node,element2NodeStart,element2VTKId);
	}
};

TEST_F(MarkerTEST, GetNElement) {
	int nElement = marker.GetNElement();
	EXPECT_EQ(nElement, 2);
}

TEST_F(MarkerTEST, AreArraysEqual_sameorder){
    // equality should be respected if the 2 arrays are identical
    int ar1[3] = {1,2,3};
    int ar2[3] = {1,2,3};
    int length = 3;
    int result = marker.AreArraysEqual(ar1, ar2,length);
    EXPECT_EQ(result, 0);
}

TEST_F(MarkerTEST, AreArraysEqual_notequal){
    // equality should not be respected if the 2 arrays are not identical
    int ar1[3] = {1,2,4};
    int ar2[3] = {1,2,3};
    int length = 3;
    int result = marker.AreArraysEqual(ar1, ar2,length);
    EXPECT_EQ(result, 1);
}

TEST_F(MarkerTEST, AreArraysEqual_first_array_unordered){
    // equality should be respected if the 2 arrays are identical and with an offset
    int ar1[3] = {2,3,1};
    int ar2[3] = {1,2,3};
    int length = 3;
    int result = marker.AreArraysEqual(ar1, ar2,length);
    EXPECT_EQ(result, 0);
}

TEST_F(MarkerTEST, Check4Face_notcontaining){
    // the face should not be added to m_containingFaces if the supplied array is
    // not found in the marker's connectivity
    int nodes[2]={10,10};
    int nnode = 2;
    int nface = 3;
    int result = marker.Check4Face(nodes, nnode, nface);
    EXPECT_EQ(result, 1);
}

TEST_F(MarkerTEST, Check4Face_containing){
    // the face should be added to m_containingFaces if the supplied array is
    // found in the marker's connectivity
    int nodes[2]={3,4};
    int nnode = 2;
    int nface = 3;
    int result = marker.Check4Face(nodes, nnode, nface);
    EXPECT_EQ(result, 0);
}