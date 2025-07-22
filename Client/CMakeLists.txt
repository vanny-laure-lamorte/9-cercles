# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.5)

# Define the project and its version
project(client VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#  Source files
set(LPTF_SRC src/LPTF_Socket.cpp)
set(CLIENT_MAIN src/client.cpp)
set(TEST_SRC test/test_socket.cpp)

#  Create a reusable static library for client logic
add_library(lptf_client STATIC ${LPTF_SRC})
target_include_directories(lptf_client PUBLIC include)
target_link_libraries(lptf_client PUBLIC ws2_32)  # Link Winsock2 on Windows

#  Main client executable
add_executable(client ${CLIENT_MAIN})
target_link_libraries(client PRIVATE lptf_client)

# Output the 'client' binary in the root project directory
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# === Unit Tests ===
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

# Test executable using Catch2
add_executable(client_test ${TEST_SRC})
target_link_libraries(client_test PRIVATE lptf_client Catch2::Catch2WithMain)

# Enable CTest and register the test
enable_testing()
add_test(NAME ClientTest COMMAND client_test)
