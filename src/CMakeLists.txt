cmake_minimum_required (VERSION 3.8)
project(ssge-server CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Add source to this project's executable.
file(GLOB source_files
    "*.h"
    "*.cpp"
)

add_library(ssge-server ${source_files})
target_link_libraries(ssge-server ${CONAN_LIBS})
