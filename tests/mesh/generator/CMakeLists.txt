set(TESTNAME test_MeshReader)                                 # Name for this test executable
set(SOURCE test_MeshReader.cpp                                # List of source files to be included in the executable
            # other cpp file
            # and an other one! 
            )
set(DEPENDANCIES MeshGeneration                                   # List of all the tests dependancies (every non standard #include )
            # Other library
            # Ouioui vous pouvez en mettre autant que necessaire
            )
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt

set(TESTNAME test_Parser)                                 # Name for this test executable
set(SOURCE test_Parser.cpp                                # List of source files to be included in the executable
            # other cpp file
            # and an other one! 
            )
set(DEPENDANCIES MeshGeneration                                   # List of all the tests dependancies (every non standard #include )
            # Other library
            # Ouioui vous pouvez en mettre autant que necessaire
            )
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt

set(TESTNAME test_MeshGenerator)                                 # Name for this test executable

set(SOURCE test_MeshGenerator.cpp)
set(DEPENDANCIES MeshGeneration Logger)
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt