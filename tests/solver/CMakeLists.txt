set(TESTNAME test_Solver)                                 # Name for this test executable
set(SOURCE test_Solver.cpp)
set(DEPENDANCIES InputParser MeshGeneration Metrics Logger PostProcessing Solver)
package_add_test(${TESTNAME} ${SOURCE} "${DEPENDANCIES}")     # Call to the macro in ./tests/CMakeLists.txt

set(TESTNAME test_Residual)                                 # Name for this test executable
set(SOURCE test_Residual.cpp)
set(DEPENDANCIES Solver)
package_add_test(${TESTNAME} ${SOURCE} "${DEPENDANCIES}")     # Call to the macro in ./tests/CMakeLists.txt
