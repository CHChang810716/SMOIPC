target_include_directories(${AKT_TARGET} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)