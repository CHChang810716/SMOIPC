akt_add_executable(${AKT_TARGET} exe_targets ${AKT_CHILD_SRCS})
include(${CMAKE_CURRENT_LIST_DIR}/basic_include.cmake)
target_link_libraries(${AKT_TARGET} PUBLIC 
    ${AKT_CHILD_TARGETS}
)
akt_vscode_add_to_launch(${AKT_TARGET})