cmake_minimum_required(VERSION 3.10)
project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) # Qt5 fonctionne mieux en 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Activer Qt Meta-Object Compiler (moc, uic, rcc) ===
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# === HEADERS & SOURCES ===
file(GLOB HEADER_FILES
"include/*.h"
)
set(SOCKET_SOURCES
    src/LPTF_Socket.cpp
    src/LPTF_Packet.cpp
    src/LPTF_Server.cpp
)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# === LIBRAIRIE serveur ===
add_library(lptf_server STATIC ${SOCKET_SOURCES} ${HEADER_FILES})
target_include_directories(lptf_server PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lptf_server PUBLIC Qt5::Core ws2_32)

# === EXECUTABLE CONSOLE ===
add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE lptf_server Qt5::Core ws2_32)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# === EXECUTABLE GUI ===
add_executable(server_gui
    src/server_gui.cpp
    src/ui/MainWindow.cpp
    src/ui/DashboardWidget.cpp
    src/ui/InfoSystemWidget.cpp
    src/ui/ProcessManagerWidget.cpp
    include/ui/MainWindow.h
    include/ui/DashboardWidget.h
    include/ui/InfoSystemWidget.h
    include/ui/ProcessManagerWidget.h
)

target_link_libraries(server_gui PRIVATE lptf_server Qt5::Widgets ws2_32)
target_include_directories(server_gui PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/ui)

set_target_properties(server_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

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