#include "../../../src/mesh/generator/MeshReaderSU2.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>

class MeshReaderSU2NewFormatTEST : public ::testing::Test {
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

TEST_F(MeshReaderSU2NewFormatTEST, ReadFile) {
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
	                                     21,
										 24};
	std::vector<int> element2NodeStart_v(element2NodeStart.get(), element2NodeStart.get() + nElement+1);
	ASSERT_THAT(element2NodeStart_v, testing::ElementsAreArray(element2NodeStart_exp));


	// std::vector<double> v(coor.get(), coor.get() + nNode*nDime);
	// ASSERT_THAT(v, testing::ElementsAreArray(expected_coord));
}

class MeshReaderSU2OLDFormatTEST : public ::testing::Test {
protected:
	MeshReaderSU2 reader = MeshReaderSU2("../../../../tests/mesh/generator/square_5x5.su2");
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

TEST_F(MeshReaderSU2OLDFormatTEST, ReadFile) {
	ASSERT_EQ(nDime, 2);
	ASSERT_EQ(nNode, 36);
	ASSERT_EQ(nElement, 25);

	const double coor_exp[] = {0.000000000000000,        0.000000000000000,
								0.000000000000000,       0.2000000000000000,
								0.000000000000000,       0.4000000000000000,
								0.000000000000000,       0.6000000000000001,
								0.000000000000000,       0.8000000000000000,
								0.000000000000000,        1.000000000000000,
								0.2000000000000000,        0.000000000000000,
								0.2000000000000000,       0.2000000000000000,
								0.2000000000000000,       0.4000000000000000,
								0.2000000000000000,       0.5999999999999999,
								0.2000000000000000,       0.7999999999999999,
								0.2000000000000000,        1.000000000000000,
								0.3999999999999999,        0.000000000000000,
								0.3999999999999999,       0.2000000000000000,
								0.4000000000000001,       0.3999999999999999,
								0.4000000000000000,       0.6000000000000001,
								0.4000000000000000,       0.8000000000000000,
								0.4000000000000000,        1.000000000000000,
								0.6000000000000000,        0.000000000000000,
								0.6000000000000000,       0.2000000000000000,
								0.6000000000000002,       0.4000000000000001,
								0.6000000000000001,       0.6000000000000000,
								0.6000000000000001,       0.8000000000000000,
								0.6000000000000001,        1.000000000000000,
								0.8000000000000000,        0.000000000000000,
								0.8000000000000000,       0.2000000000000000,
								0.8000000000000000,       0.3999999999999999,
								0.8000000000000000,       0.6000000000000000,
								0.8000000000000000,       0.8000000000000000,
								0.8000000000000000,        1.000000000000000,
								1.000000000000000,        0.000000000000000,
								1.000000000000000,       0.2000000000000000,
								1.000000000000000,       0.3999999999999999,
								1.000000000000000,       0.6000000000000000,
								1.000000000000000,       0.8000000000000000,
								1.000000000000000,        1.000000000000000};
	std::vector<double> coor_v(coor.get(), coor.get() + nNode * nDime);
	ASSERT_THAT(coor_v, testing::ElementsAreArray(coor_exp));

	const int element2Node_exp[] = {0,1,7,6,
									1,2,8,7,
									2,3,9,8,
									3,4,10,9,
									4,5,11,10,
									6,7,13,12,
									7,8,14,13,
									8,9,15,14,
									9,10,16,15,
									10,11,17,16,
									12,13,19,18,
									13,14,20,19,
									14,15,21,20,
									15,16,22,21,
									16,17,23,22,
									18,19,25,24,
									19,20,26,25,
									20,21,27,26,
									21,22,28,27,
									22,23,29,28,
									24,25,31,30,
									25,26,32,31,
									26,27,33,32,
									27,28,34,33,
									28,29,35,34};
	std::vector<int> element2Node_v(element2Node.get(), element2Node.get() + nElement * 4);
	ASSERT_THAT(element2Node_v, testing::ElementsAreArray(element2Node_exp));

	const int element2NodeStart_exp[] = {0,
											4,
											8,
											12,
											16,
											20,
											24,
											28,
											32,
											36,
											40,
											44,
											48,
											52,
											56,
											60,
											64,
											68,
											72,
											76,
											80,
											84,
											88,
											92,
											96,
											100};
	std::vector<int> element2NodeStart_v(element2NodeStart.get(), element2NodeStart.get() + nElement+1);
	ASSERT_THAT(element2NodeStart_v, testing::ElementsAreArray(element2NodeStart_exp));


	// std::vector<double> v(coor.get(), coor.get() + nNode*nDime);
	// ASSERT_THAT(v, testing::ElementsAreArray(expected_coord));
}

