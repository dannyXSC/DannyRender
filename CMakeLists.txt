cmake_minimum_required(VERSION 3.10)
project(DannyRender)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rd/tinyxml2)

include_directories("3rd")
include_directories("src")
include_directories("3rd/tinyxml2")

link_directories("${CMAKE_CURRENT_BINARY_DIR}/3rd/tinyxml2/")
link_directories("${CMAKE_CURRENT_BINARY_DIR}/3rd/tinyxml2/debug/")
link_directories("${CMAKE_CURRENT_BINARY_DIR}/3rd/tinyxml2/release/")


find_package(glm CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -O3 -fopenmp")

file(GLOB_RECURSE SOURCE_FILES  "src/main.cpp" 
                                "src/core/*.cpp"  "src/core/*.hpp" "src/core/*.h" 
                                "src/geometry/*.cpp" "src/geometry/*.hpp" "src/geometry/*.h"
                                "src/integrator/*.cpp" "src/integrator/*.hpp" "src/integrator/*.h"
                                "src/light/*.cpp" "src/light/*.hpp" "src/light/*.h"
                                "src/material/*.cpp" "src/material/*.hpp" "src/material/*.h"
                                "src/microfacet/*.cpp" "src/microfacet/*.hpp" "src/microfacet/*.h"
                                "src/texture/*.cpp" "src/texture/*.hpp" "src/texture/*.h"
                                "src/utils/*.cpp" "src/utils/*.hpp" "src/utils/*.h"
                                "src/xml/*.cpp" "src/xml/*.hpp" "src/xml/*.h")

add_executable(DannyRender ${SOURCE_FILES})

target_link_libraries(DannyRender glm::glm)

string(CONCAT TINYXML2_IMPORT_DEBUG "${CMAKE_IMPORT_LIBRARY_PREFIX}" "tinyxml2d" "${CMAKE_IMPORT_LIBRARY_SUFFIX}")
string(CONCAT TINYXML2_IMPORT_RELEASE "${CMAKE_IMPORT_LIBRARY_PREFIX}" "tinyxml2" "${CMAKE_IMPORT_LIBRARY_SUFFIX}")

target_link_libraries(DannyRender
        debug ${TINYXML2_IMPORT_DEBUG}
        optimized ${TINYXML2_IMPORT_RELEASE})