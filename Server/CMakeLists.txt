# Specify the miniimum CMake version required
cmake_minimum_required(VERSION 3.5)

# Define the project name and version
project(server)
set(PROJECT_VERSION "v1.0")

# Enable automatic handling of Qt-specific features
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .h files in the include folder
file(GLOB INCLUDE_FOLDER
"include/*.h"
)

# Collect all .cpp files in the src folder
file(GLOB SRC_FOLDER
"src/*.cpp"
)

# Combine the include folder and source files into a single list
set (SRCS
    ${INCLUDE_FOLDER}
    ${SRC_FOLDER}
)

# Create the 'client' binary using all collected source files
add_executable(server ${SRCS})

# Link against Winsock2 on Windows
target_link_libraries(server ws2_32)

# Place the compiled 'client' binary in the root directory of the project
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
