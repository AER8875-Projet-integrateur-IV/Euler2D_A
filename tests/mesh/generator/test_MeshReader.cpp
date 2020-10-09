#include "../../../src/mesh/generator/MeshReaderSU2.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>

class MeshReaderSU2TEST : public ::testing::Test {
protected:
	MeshReaderSU2 reader = MeshReaderSU2("../../../../tests/mesh/generator/square.su2");
	int nDime;
	int nNode;
	int nElement;
	std::unique_ptr<double[]> coor = std::unique_ptr<double[]>(NULL);
	std::unique_ptr<int[]> element2Node = std::unique_ptr<int[]>(NULL);
	std::unique_ptr<int[]> element2NodeStart = std::unique_ptr<int[]>(NULL);

	void SetUp() override {
		reader.ReadFile();
		reader.get_values(&nDime, &nNode, &nElement, &coor, &element2Node, &element2NodeStart);
	}
};

TEST_F(MeshReaderSU2TEST, ReadFile) {
	ASSERT_EQ(nDime, 2);
	ASSERT_EQ(nNode, 9);
	ASSERT_EQ(nElement, 8);

	const double coor_exp[] = {10.1, 0.0,
	                           0.5, 0.0,
	                           1.0, 0.0,
	                           0.0, 0.5,
	                           0.5, 0.5,
	                           1.0, 0.5,
	                           0.0, 1.0,
	                           0.5, 1.0,
	                           1.0, 1.0};
	std::vector<double> coor_v(coor.get(), coor.get() + nNode * nDime);
	ASSERT_THAT(coor_v, testing::ElementsAreArray(coor_exp));

	const int element2Node_exp[] = {0, 1, 3,
	                                1, 4, 3,
	                                1, 2, 4,
	                                2, 5, 4,
	                                3, 4, 6,
	                                4, 7, 6,
	                                4, 5, 7,
	                                5, 8, 7};
	std::vector<int> element2Node_v(element2Node.get(), element2Node.get() + nElement * 3);
	ASSERT_THAT(element2Node_v, testing::ElementsAreArray(element2Node_exp));

	const int element2NodeStart_exp[] = {0,
	                                     3,
	                                     6,
	                                     9,
	                                     12,
	                                     15,
	                                     18,
	                                     21};
	std::vector<int> element2NodeStart_v(element2NodeStart.get(), element2NodeStart.get() + nElement);
	ASSERT_THAT(element2NodeStart_v, testing::ElementsAreArray(element2NodeStart_exp));


	// std::vector<double> v(coor.get(), coor.get() + nNode*nDime);
	// ASSERT_THAT(v, testing::ElementsAreArray(expected_coord));
}
