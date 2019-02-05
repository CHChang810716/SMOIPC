include(cmake/scheme/test.cmake)
if(BUILD_TEST)
    target_link_libraries(${AKT_TARGET} PRIVATE opsism-fibonacci)
endif()