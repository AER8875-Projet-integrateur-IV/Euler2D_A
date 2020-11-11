set(TESTNAME test_MetricsGenerator)                                 # Name for this test executable
set(SOURCE test_MetricsGenerator.cpp)
set(DEPENDANCIES MeshGeneration Metrics Logger)
package_add_test(${TESTNAME} ${SOURCE} "${DEPENDANCIES}")     # Call to the macro in ./tests/CMakeLists.txt
