include(cmake/scheme/test.cmake)
if(WIN32)
    target_link_libraries(${AKT_TARGET} PRIVATE Boost::system ws2_32)
endif()