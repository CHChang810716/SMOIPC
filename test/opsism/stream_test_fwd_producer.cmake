include(cmake/scheme/test.cmake)
if(WIN32)
    target_link_libraries(${AKT_TARGET} PRIVATE ws2_32)
endif()