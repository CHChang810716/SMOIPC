include(cmake/scheme/test.cmake)
target_link_libraries(${AKT_TARGET} PRIVATE Boost::system ws2_32 Boost::filesystem)