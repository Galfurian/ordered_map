# -----------------------------------------------------------------------------
# @brief  : Test directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

add_executable(ordered_map_test ${CMAKE_CURRENT_SOURCE_DIR}/ordered_map_test.cpp)
target_include_directories(ordered_map_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ordered_map_test ordered_map)
add_test(NAME ordered_map_test_run_0 COMMAND ordered_map_test 0)
add_test(NAME ordered_map_test_run_1 COMMAND ordered_map_test 1)
add_test(NAME ordered_map_test_run_2 COMMAND ordered_map_test 2)
add_test(NAME ordered_map_test_run_3 COMMAND ordered_map_test 3)
add_test(NAME ordered_map_test_run_4 COMMAND ordered_map_test 4)