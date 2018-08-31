cmake_minimum_required(VERSION 2.8.12)

project(QMLConfigEditor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc" "configreader.cpp" "configreader.h" "configmanager.cpp" "configmanager.h" "yamltree.cpp" "yamltree.h" "misc.cpp" "misc.h")

INCLUDE_DIRECTORIES(libyaml/include)
INCLUDE_DIRECTORIES(libyaml/src)
LINK_DIRECTORIES(libyaml/bin)

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/libyaml/bin/libyaml.lib)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
