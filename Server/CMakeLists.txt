# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.5)

# Define the project and its version
project(server VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Source files ===
set(LPTF_SRC src/LPTF_Socket.cpp)
set(SERVER_MAIN src/server.cpp)
set(TEST_SRC test/test_socket.cpp)

# === Create a reusable static library for server logic ===
add_library(lptf_server STATIC ${LPTF_SRC})
target_include_directories(lptf_server PUBLIC include)
target_link_libraries(lptf_server PUBLIC ws2_32)

# === Main server executable ===
add_executable(server ${SERVER_MAIN})
target_link_libraries(server PRIVATE lptf_server)

# Output the 'server' binary in the root project directory
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# === Unit Tests ===
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

# Test executable using Catch2
add_executable(server_test ${TEST_SRC})
target_link_libraries(server_test PRIVATE lptf_server Catch2::Catch2WithMain)

# Enable CTest and register the test
enable_testing()
add_test(NAME ServerTest COMMAND server_test)
