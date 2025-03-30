cmake_minimum_required(VERSION 3.26)
project(Prog4-Digger)

set(CMAKE_CXX_STANDARD 20)


# Add the subdirectories
add_subdirectory(Prog4Engine)
add_subdirectory(Prog4Game)

target_compile_features(Prog4-Game PUBLIC cxx_std_20)

target_link_libraries(Prog4-Game PRIVATE Prog4-Engine)