add_executable(${CMAKE_PROJECT_NAME} main.cpp)

add_subdirectory(mesh)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Mesh)