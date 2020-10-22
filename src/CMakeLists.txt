add_executable(${CMAKE_PROJECT_NAME} main.cpp)

add_subdirectory(mesh)
add_subdirectory(utils)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC MeshGeneration Logger)
