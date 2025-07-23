cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === HEADERS & SOURCES ===
file(GLOB HEADER_FILES "include/*.h")
set(SOCKET_SOURCES
src/LPTF_Socket.cpp
src/LPTF_Packet.cpp
src/LPTF_Client.cpp)

# === LIBRAIRIE : lptf_client
add_library(lptf_client STATIC ${SOCKET_SOURCES} ${HEADER_FILES})
target_include_directories(lptf_client PUBLIC include)

# === PRINCIPAL : client
add_executable(client src/client.cpp)
target_link_libraries(client PRIVATE lptf_client ws2_32)
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# === TESTS ===
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(client_tests test/test_socket.cpp)
target_link_libraries(client_tests PRIVATE lptf_client Catch2::Catch2WithMain ws2_32)
target_include_directories(client_tests PRIVATE include)

enable_testing()
add_test(NAME ClientTests COMMAND client_tests)