cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# === QT ===
find_package(Qt6 COMPONENTS Widgets REQUIRED)


# === HEADERS & SOURCES ===
file(GLOB HEADER_FILES "include/*.h")

set(SOCKET_SOURCES
src/LPTF_Socket.cpp
src/LPTF_Packet.cpp
src/LPTF_Server.cpp
src/mainwindow.cpp
)

# === LIBRAIRIES : lptf_server
add_library(lptf_server STATIC
    ${SOCKET_SOURCES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
    src/mainwindow.ui
    resources.qrc
)

target_include_directories(lptf_server PUBLIC include)
target_link_libraries(lptf_server PUBLIC Qt6::Widgets)
qt6_add_resources(RESOURCES resources.qrc)

# === PRINCIPAL : server
add_executable(server
    src/server.cpp
    ${RESOURCES}
   )

target_link_libraries(server PRIVATE lptf_server ws2_32)

set_target_properties(server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# # === TESTS ===
# include(FetchContent)
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG v3.5.4
# )
# FetchContent_MakeAvailable(Catch2)

# add_executable(server_tests 
# test/test_socket.cpp
# test/test_packet.cpp)
# target_link_libraries(server_tests PRIVATE lptf_server Catch2::Catch2WithMain ws2_32)
# target_include_directories(server_tests PRIVATE include)

# enable_testing()
# add_test(NAME ServerTests COMMAND server_tests)
