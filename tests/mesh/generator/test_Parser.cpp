#include "../../../src/mesh/generator/Parser.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class ParserTEST : public ::testing::Test {
protected:
	Parser parser = Parser("../../../../tests/mesh/generator/square.su2");
};

TEST_F(ParserTEST, GetNextWord) {
	std::string val = parser.GetNextWord();
	EXPECT_EQ(val, "NDIME=");

	val = parser.GetNextWord();
	EXPECT_EQ(val, "2");

	val = parser.GetNextWord();
	EXPECT_EQ(val, "NPOIN=");
}

TEST_F(ParserTEST, GetNextNonNullLine) {
	std::string val = parser.GetNextNonNullLine();
	EXPECT_EQ(val, "NDIME= 2");
}

TEST_F(ParserTEST, ExtractNextInt) {
	int val = parser.ExtractNextInt();
	EXPECT_EQ(val, 2);

	val = parser.ExtractNextInt();
	EXPECT_EQ(val, 9);

	val = parser.ExtractNextInt();
	EXPECT_EQ(val, 8);
}

TEST_F(ParserTEST, ExtractNextDouble) {
	double val = parser.ExtractNextDouble();
	EXPECT_EQ(val, 10.1);

	val = parser.ExtractNextDouble();
	EXPECT_EQ(val, 0.0);
}