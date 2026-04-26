cmake_minimum_required(VERSION 4.0)

project(alltomd)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclui headers próprios
include_directories(${PROJECT_SOURCE_DIR}/include)

# Coleta todos os .cpp
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Libs e pacotes
find_package(nlohmann_json CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)

FetchContent_MakeAvailable(cli11_proj)

# Executável
add_executable(alltomd ${SOURCES})

target_link_libraries(alltomd PRIVATE 
    nlohmann_json::nlohmann_json  
    CLI11::CLI11
)

message(STATUS "Sources found: ${SOURCES}")