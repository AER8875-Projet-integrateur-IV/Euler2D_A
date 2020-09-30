#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./mesh/test_MeshReader.cpp"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}