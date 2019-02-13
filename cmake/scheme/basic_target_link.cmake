target_link_libraries(${AKT_TARGET} PRIVATE Threads::Threads)
if(UNIX)
    target_link_libraries(${AKT_TARGET} PRIVATE ${CMAKE_DL_LIBS} rt)
endif()
