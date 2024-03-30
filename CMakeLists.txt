cmake_minimum_required(VERSION 3.10)
project(DannyRender)

include_directories("3rd")
include_directories("src")

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
                                "src/utils/*.cpp" "src/utils/*.hpp" "src/utils/*.h")

add_executable(DannyRender ${SOURCE_FILES})

target_link_libraries(DannyRender PRIVATE glm::glm)
