cmake_minimum_required(VERSION 3.18)
project(fluidSim VERSION 0.1.0)

add_executable(fluidSim
    src/main.cpp
    src/utils/constatns.cpp
    src/utils/functions.cpp
    src/utils/shadersInit.cpp
    src/classes/Circle.cpp
    src/classes/Water.cpp
)

include(FetchContent)
FetchContent_Declare(soil2  
    GIT_REPOSITORY https://github.com/SpartanJ/SOIL2  
    GIT_TAG master  
)  
FetchContent_MakeAvailable(soil2)

add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

target_include_directories(fluidSim PRIVATE
    ${CMAKE_SOURCE_DIR}/core
)

target_include_directories(fluidSim PRIVATE  
    ${soil2_SOURCE_DIR}/src  
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(fluidSim PRIVATE soil2 glad glfw OpenGL::GL glm::glm)

target_compile_definitions(fluidSim PRIVATE CMAKE_SOURCE="${CMAKE_SOURCE_DIR}/")
