include(cmake/scheme/test.cmake)
target_link_libraries(${AKT_TARGET} PRIVATE 
    Boost::system 
    Boost::serialization
    Boost::filesystem
)
if(WIN32)
    target_link_libraries(${AKT_TARGET} PRIVATE ws2_32)
endif()