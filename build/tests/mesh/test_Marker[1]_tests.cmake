add_test( MarkerTEST.GetNElement /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/test_Marker [==[--gtest_filter=MarkerTEST.GetNElement]==] --gtest_also_run_disabled_tests)
set_tests_properties( MarkerTEST.GetNElement PROPERTIES WORKING_DIRECTORY /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh VS_DEBUGGER_WORKING_DIRECTORY)
set( test_Marker_TESTS MarkerTEST.GetNElement)
