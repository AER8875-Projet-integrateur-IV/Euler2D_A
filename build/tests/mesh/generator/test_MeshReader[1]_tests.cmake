add_test( MeshReaderSU2NewFormatTEST.ReadFile /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator/test_MeshReader [==[--gtest_filter=MeshReaderSU2NewFormatTEST.ReadFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( MeshReaderSU2NewFormatTEST.ReadFile PROPERTIES WORKING_DIRECTORY /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/generator VS_DEBUGGER_WORKING_DIRECTORY)
set( test_MeshReader_TESTS MeshReaderSU2NewFormatTEST.ReadFile)
