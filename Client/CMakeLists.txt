cmake_minimum_required(VERSION 3.5)
project(client)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Récupérer tous les .cpp  dans src et .h include
file(GLOB INCLUDE_FOLDER
    "include/*.cpp"
    "include/*.h"
)

file(GLOB SRC_FOLDER
    "src/*.cpp"
    "src/*.h"
)
# Regroupe tout dans SRCS
set (SRCS
    ${INCLUDE_FOLDER}
    ${SRC_FOLDER}
)

add_executable(client ${SRCS})
