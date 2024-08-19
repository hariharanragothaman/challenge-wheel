cmake_minimum_required(VERSION 3.28)
project(wheel)
set(CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE_DIR "/opt/homebrew/include/SFML")
include_directories(${SFML_INCLUDE_DIR})

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(wheel ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(wheel PRIVATE sfml-graphics)
