project(Babel)
cmake_minimum_required(VERSION 2.8.12)
add_definitions("-fPIC")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS client/ui)

include_directories(${CMAKE_INCLUDE_PATH})

add_executable(babel_server src/main.cpp src/Parser.cpp src/db.cpp src/session.cpp src/server.cpp)
add_executable(babel_client client/src/main.cpp client/src/babelfriend.cpp client/src/login.cpp client/src/mainwindow.cpp client/include/babelfriend.h client/include/login.h client/include/mainwindow.h client/include/client.h client/src/client.cpp client/include/tcpNetwork.h client/src/tcpNetwork.cpp client/include/udpNetwork.h client/src/udpNetwork.cpp client/src/Compressor.cpp client/src/Audio.cpp client/include/Audio.hpp client/include/Compressor.hpp client/include/ISound.hpp client/ressources.qrc)

target_link_libraries(babel_server ${CONAN_LIBS} Qt5::Widgets)
target_link_libraries(babel_client ${CONAN_LIBS} Qt5::Widgets Qt5::Network)