cmake_minimum_required(VERSION 3.5)
project(client)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Récupérer tous les .cpp dans src
file(GLOB SRC_FOLDER
    "src/*.cpp"
)

add_executable(client ${SRC_FOLDER})

# Ajouter le dossier include comme répertoire d'includes
target_include_directories(client PRIVATE "${PROJECT_SOURCE_DIR}/include")

# 🔧 Link against Winsock2 on Windows
target_link_libraries(client ws2_32)
