cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === HEADERS & SOURCES ===
file(GLOB HEADER_FILES "include/*.h")
set(SOCKET_SOURCES src/LPTF_Socket.cpp)

# === LIBRAIRIES : lptf_server
add_library(lptf_server STATIC ${SOCKET_SOURCES} ${HEADER_FILES})
target_include_directories(lptf_server PUBLIC include)

# === PRINCIPAL : server
add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE lptf_server ws2_32)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# === TESTS ===
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(server_tests tests/tests_server.cpp)
target_link_libraries(server_tests PRIVATE lptf_server Catch2::Catch2WithMain ws2_32)
target_include_directories(server_tests PRIVATE include)

enable_testing()
add_test(NAME ServerTests COMMAND server_tests)
