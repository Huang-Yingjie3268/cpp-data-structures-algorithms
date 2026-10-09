cmake_minimum_required(VERSION 3.16)
project(selected_cpp_algorithms LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(prime_path_bfs src/graph/prime_path_bfs.cpp)
add_executable(dynamic_quadtree src/tree/dynamic_quadtree.cpp)
add_executable(fleet_distance src/disjoint_set/fleet_distance.cpp)
add_executable(running_median src/heap/running_median.cpp)
add_executable(lru_cache src/cache/lru_cache.cpp)
add_executable(coin_flipping src/search/coin_flipping.cpp)
add_executable(customs src/sliding_window/customs.cpp)


include(CTest)
find_package(Python3 COMPONENTS Interpreter)
if(BUILD_TESTING AND Python3_Interpreter_FOUND)
  add_test(NAME regression COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/verify.py --bin-dir $<TARGET_FILE_DIR:prime_path_bfs>)
endif()
