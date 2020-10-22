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
        std::unique_ptr<int[]> element2NodeStart = std::unique_ptr<int[]>(new int[3]);
        for (int i=0;i<3;i++){
            element2NodeStart[i] = element2NodeStart_val[i]; 
        }      

        int element2Node_val[4] = {1,2,3,4};
        std::unique_ptr<int[]> element2Node = std::unique_ptr<int[]>(new int[4]);
        for (int i=0;i<4;i++){
            element2Node[i] = element2Node_val[i]; 
        }    

        marker.Set(&tag,&nElement,&element2Node,&element2NodeStart);
	}
};

TEST_F(MarkerTEST, GetNElement) {
	int nElement = marker.GetNElement();
	EXPECT_EQ(nElement, 2);
}
