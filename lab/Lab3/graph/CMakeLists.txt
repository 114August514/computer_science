# Graph CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

# Set include directory
include_directories(include)

# Source files
set(GRAPH_SOURCES
    src/graph.cpp
    src/dijkstra.cpp
)

# Header files (for IDE support)
set(GRAPH_HEADERS
    include/graph.h
    include/dijkstra.h
)

# Create executable
add_executable(GraphDemo
    src/main.cpp
    ${GRAPH_SOURCES}
    ${GRAPH_HEADERS}
)

# Set target properties
set_target_properties(GraphDemo PROPERTIES
    OUTPUT_NAME "GraphDemo"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)