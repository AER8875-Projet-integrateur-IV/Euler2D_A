#include <gtest/gtest.h>
#include <gmock/gmock.h>
// #include "../../src/mesh/MeshReader.hpp"


TEST(MeshTest, supposed_to_fail) {
    ASSERT_THAT(0, 1); // This statement is supposed to fail to showcase a failed test
}

TEST(MeshTest, supposed_to_succed) {
    ASSERT_THAT(1, 1); // This statement is supposed to fail to showcase a failed test
}


// to compile a test file :     g++ -o [output_file] [test_file] [file.cpp] -lgtest -lgmock -lpthread
// for this file:               g++ -o Run_Test test/Grid/test_Mesh.cpp src/Grid/Mesh.cpp -lgtest -lgmock -lpthread

