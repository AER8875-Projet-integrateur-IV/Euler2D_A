
add_subdirectory(metrics)
add_subdirectory(generator)

set(TESTNAME test_Marker)                                 # Name for this test executable
set(SOURCE test_Marker.cpp)
set(DEPENDANCIES Mesh Logger)
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt