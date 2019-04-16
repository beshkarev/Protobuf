#cmake_minimum_required(VERSION 3.8)
#project ( Protobuf )

## declaring files in your project

#find_package(Protobuf REQUIRED)
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Network REQUIRED)
#find_package(Qt5PrintSupport REQUIRED)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC OFF)

#include_directories(${PROTOBUF_INCLUDE_DIRS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
##include_directories(${Qt5Widgets_INCLUDE_DIRS})
##include_directories(${Qt5Core_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(${QT_INCLUDES})

#set (ProtoFiles
#  proto/Message.proto
#)

#set ( SOURCES
# src/main.cpp
# src/mainwindow.cpp
# #src/MemoryWorker.cpp
# src/ipc/socket.cpp
# src/ipc/MessageDispatcher.cpp

# ${ProtoFiles}
#)

#set ( HEADERS
# #src/MemoryWorker.h
# src/Logger.hpp
# src/ipc/MessageDispatcher.h
# src/ipc/socket.h
#)

#set ( MOC_HEADERS
# src/mainwindow.h
# src/ipc/socket.h
#)

#set ( UIS
# UI/mainwindow.ui
#)

#set_property(SOURCE src/mainwindow.h PROPERTY SKIP_AUTOMOC ON)

#set ( RESOURCES
## main.qrc
#)

## Next lines needed for building all Qt projects
#find_package( Qt4 REQUIRED )
#include( ${QT_USE_FILE} )
#add_definitions( ${QT_DEFINITIONS} )
#include_directories( ${CMAKE_BINARY_DIR} )

## Next, using precompiler, compiler and linker

## using Qt meta-system (precompiler)
#QT4_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
#QT4_WRAP_UI( UI_HEADERS ${UIS} )
#QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

#PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

## compile
#add_executable( Protobuf ${SOURCES} ${HEADERS} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} ${PROTO_SRCS} ${PROTO_HDRS})
## or use line below instead, if you using Windows ™ Operating System.
##add_executable( PROJECT_NAME WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )

#set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)

## build it (link libraries)
#target_link_libraries( ${PROJECT_NAME} ${QT_LIBRARIES} ${QT_GUI_LIBRARIES} Qt5::Network ${PROTOBUF_LIBRARY} )

cmake_minimum_required(VERSION 3.8)

project(Protobuf)

find_package(Protobuf REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set (ProtoFiles
  proto/Message.proto
)

set ( SOURCES
 src/main.cpp
 src/mainwindow.cpp
 #src/MemoryWorker.cpp
 src/ipc/socket.cpp
 src/ipc/message_dispatcher.cpp
 src/ipc/socket_impl.cpp
 src/ipc/dispatchers/editor_dispatcher.cpp
 src/ipc/dispatchers/server_dispatcher.cpp
 src/ipc/tcp_server.cpp

 ${ProtoFiles}
)

set ( HEADERS
 #src/MemoryWorker.h
 src/Logger.hpp
 src/configuration.hpp
 src/ipc/message_dispatcher.h
 src/ipc/socket.h
 src/ipc/socket_impl.h
 src/ipc/dispatchers/editor_dispatcher.h
 src/ipc/dispatchers/server_dispatcher.h
 src/ipc/tcp_server.h
)

set ( UIS
 UI/mainwindow.ui
)

qt5_wrap_ui( UI_HEADERS ${UIS} )

set(PROJECT_SRCS ${SOURCES})
set(PROJECT_HDRS ${HEADERS})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})
add_executable(${PROJECT_NAME} ${PROJECT_SRCS} ${UI_HEADERS} ${PROJECT_HDRS} ${PROTO_SRCS} ${PROTO_HDRS})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Widgets ${PROTOBUF_LIBRARIES})
