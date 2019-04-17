cmake_minimum_required(VERSION 3.8)

project(Protobuf)

include(FindProtobuf)

find_package(Protobuf 3.5 REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets)
find_package(Boost 1.40 REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROTOBUF_LIBS /usr/local/lib/libprotobuf.a)

set (ProtoFiles
  proto/Message.proto
)

set ( SOURCES
 src/main.cpp
 src/mainwindow.cpp
 #src/MemoryWorker.cpp
 src/logging/logger.cpp
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
 src/logging/logger.h
 src/logging/file_worker.hpp
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

set ( RESOURCES
 resources/resources.qrc
)

qt5_wrap_ui( UI_HEADERS ${UIS} )

set(PROJECT_SRCS ${SOURCES})
set(PROJECT_HDRS ${HEADERS})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})
add_executable(${PROJECT_NAME} ${PROJECT_SRCS} ${UI_HEADERS} ${PROJECT_HDRS} ${PROTO_SRCS} ${PROTO_HDRS} ${RESOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Widgets ${PROTOBUF_LIBS} -lpthread)
