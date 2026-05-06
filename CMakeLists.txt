cmake_minimum_required(VERSION 3.20)

project(proj VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CLI11 CONFIG QUIET)

if(NOT CLI11_FOUND)
    message(STATUS "CLI11 not found via package config. On macOS/Homebrew, you can pass -DCMAKE_PREFIX_PATH=$(brew --prefix cli11).")
    include(FetchContent)
    FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.6.2
    )
    FetchContent_MakeAvailable(cli11)
endif()

find_package(yaml-cpp CONFIG QUIET)

if(NOT yaml-cpp_FOUND)
    message(STATUS "yaml-cpp not found via package config. On macOS/Homebrew, you can pass -DCMAKE_PREFIX_PATH=$(brew --prefix yaml-cpp).")
    include(FetchContent)
    FetchContent_Declare(
        yaml_cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.9.0
    )
    FetchContent_MakeAvailable(yaml_cpp)
endif()


add_executable(proj
    src/utility.cpp
    src/cli.cpp
    src/config_settings.cpp
    src/main.cpp
)

target_link_libraries(proj
    PRIVATE
        CLI11::CLI11
)

if(TARGET yaml-cpp::yaml-cpp)
    target_link_libraries(proj PRIVATE yaml-cpp::yaml-cpp)
else()
    target_link_libraries(proj PRIVATE yaml-cpp)
endif()

target_include_directories(proj
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
