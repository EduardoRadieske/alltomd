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

find_package(nlohmann_json CONFIG REQUIRED)

# Executável
add_executable(alltomd ${SOURCES})

target_link_libraries(alltomd PRIVATE nlohmann_json::nlohmann_json)