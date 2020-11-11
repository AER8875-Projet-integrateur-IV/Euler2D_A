add_test( ParserTEST.GetNextWord /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator/test_Parser [==[--gtest_filter=ParserTEST.GetNextWord]==] --gtest_also_run_disabled_tests)
set_tests_properties( ParserTEST.GetNextWord PROPERTIES WORKING_DIRECTORY /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator VS_DEBUGGER_WORKING_DIRECTORY)
add_test( ParserTEST.GetNextNonNullLine /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator/test_Parser [==[--gtest_filter=ParserTEST.GetNextNonNullLine]==] --gtest_also_run_disabled_tests)
set_tests_properties( ParserTEST.GetNextNonNullLine PROPERTIES WORKING_DIRECTORY /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator VS_DEBUGGER_WORKING_DIRECTORY)
add_test( ParserTEST.ExtractNextInt /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator/test_Parser [==[--gtest_filter=ParserTEST.ExtractNextInt]==] --gtest_also_run_disabled_tests)
set_tests_properties( ParserTEST.ExtractNextInt PROPERTIES WORKING_DIRECTORY /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator VS_DEBUGGER_WORKING_DIRECTORY)
add_test( ParserTEST.ExtractNextDouble /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator/test_Parser [==[--gtest_filter=ParserTEST.ExtractNextDouble]==] --gtest_also_run_disabled_tests)
set_tests_properties( ParserTEST.ExtractNextDouble PROPERTIES WORKING_DIRECTORY /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator VS_DEBUGGER_WORKING_DIRECTORY)
set( test_Parser_TESTS ParserTEST.GetNextWord ParserTEST.GetNextNonNullLine ParserTEST.ExtractNextInt ParserTEST.ExtractNextDouble)
