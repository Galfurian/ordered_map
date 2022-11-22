# -----------------------------------------------------------------------------
# @brief  : Examples directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

add_executable(ordered_map_example ${PROJECT_SOURCE_DIR}/examples/example.cpp)
target_include_directories(ordered_map_example PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ordered_map_example ordered_map)