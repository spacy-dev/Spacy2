project(Spacy-Examples-Scalar-Newton)

cmake_minimum_required(VERSION 3.1)

# Set path to cmake scripts
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake
    )

# Do not allow in-source builds
include(ConfigSafeGuards)

find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD ${Spacy_CXX_STANDARD})

add_executable(newton newton.cpp)
target_link_libraries(newton ${Spacy_LIBRARIES})

