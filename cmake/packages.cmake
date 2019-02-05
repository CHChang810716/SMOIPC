hunter_add_package(Arkitekto)
find_package(Arkitekto CONFIG REQUIRED)

if(BUILD_TEST)
    hunter_add_package(GTest)
    find_package(GTest CONFIG REQUIRED)
endif()

hunter_add_package(Boost COMPONENTS
    system 
    filesystem
    serialization
)
find_package(Boost CONFIG COMPONENTS
    system 
    filesystem
    serialization

    REQUIRED
)