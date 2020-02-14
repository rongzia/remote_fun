cmake_minimum_required(VERSION 3.10)

message("---------------------------- start MULTI_MASTER_ZHANG")
message("building remote_fun")
if (NOT NO_DEBUG_REMOTE_SINGLE)
    message("debug remote fun single")
    ADD_DEFINITIONS(-DRAPIDJSON_NO_SIZETYPEDEFINE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../extra/multi_master/include) # for macro
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../extra/multi_master/easy_logger/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)   # for my_rapidjson_size_t.h
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../extra/rapidjson/include) # for rapidjson
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    # for libevent
    set(multi_net_io ${CMAKE_CURRENT_SOURCE_DIR}/../../../extra/multi_master/remote_fun/lib/libmulti_net_io.a)
    set(event ${CMAKE_CURRENT_SOURCE_DIR}/../../../extra/multi_master/remote_fun/lib/libevent.a)

    # for grpc
    set(PROTOBUF_ROOT $ENV{HOME}/.local/protobuf-3.6.1)
    include(cmake/protobuf.cmake)
    set(GRPC_ROOT $ENV{HOME}/.local/grpc-v1.23.x)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/grpc.cmake)
    find_library(ZLIB_LIBRARY libz.so /usr/lib/x86_64-linux-gnu/)
endif ()

add_definitions(-DMULTI_MASTER_ZHANG_LOG)
#add_definitions(-DMULTI_MASTER_ZHANG_LOG_FUN)
#add_definitions(-DMULTI_MASTER_ZHANG_LOG_JSON)

# for libevent
#set(REMOTE_FUN_WITH_LIBEVENT "1")
#if (REMOTE_FUN_WITH_LIBEVENT STREQUAL "1")
#    message("use libevent")
#    add_definitions(-DREMOTE_FUN_WITH_LIBEVENT)
#
#    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../multi-master-tool/multi_net_io/include/)   # for libevent_handle.h
##    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../extra/libevent.bak/include) # for libevent
#
#    set(REMOTE_SRCS
#            src/remote_client.cpp src/remote_server.cpp
#            src/handle_json.cpp src/handle_struct.cpp src/remote_type.cpp
#            src/handle_net.cpp src/handle_libevent_client.cpp src/handle_libevent_server.cpp)
#
#    add_library(remote_fun STATIC ${REMOTE_SRCS})
#    target_link_libraries(remote_fun ${multi_net_io} ${event} pthread)
#
#    add_executable(run_server test/run_server.cpp)
#    add_executable(test_client1 test/test_client1.cpp)
#    add_executable(test_client2 test/test_client2.cpp)
#    target_link_libraries(run_server remote_fun) # multi_net_io event pthread)
#    target_link_libraries(test_client1 remote_fun) # multi_net_io event pthread)
#    target_link_libraries(test_client2 remote_fun) # multi_net_io event pthread)
#endif()


# for grpc
set(REMOTE_FUN_WITH_GRPC "1")
if (REMOTE_FUN_WITH_GRPC STREQUAL "1")
    message("use grpc")
    add_definitions(-DREMOTE_FUN_WITH_GRPC)

    message("find PROTOBUF_LIBRARY : " ${PROTOBUF_LIBRARY})
    message("find PROTOBUF_PROTOC_EXECUTABLE : " ${PROTOBUF_PROTOC_EXECUTABLE})
    message("find GRPC_STATIC_LIBRARY : " ${GRPC_STATIC_LIBRARY})
    message("find ZLIB_LIBRARY : " ${ZLIB_LIBRARY})

    # 生成 *.grpc.pb.[h|cc], *.pb.[h|cc] ${Protobuf_PROTOC_EXECUTABLE}
    execute_process(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
            -I ${CMAKE_CURRENT_SOURCE_DIR}/proto
            --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/proto
            ${CMAKE_CURRENT_SOURCE_DIR}/proto/json.proto)
    execute_process(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
            -I ${CMAKE_CURRENT_SOURCE_DIR}/proto
            --grpc_out=${CMAKE_CURRENT_SOURCE_DIR}/proto
            --plugin=protoc-gen-grpc=$ENV{HOME}/.local/grpc-v1.23.x/bin/grpc_cpp_plugin
            ${CMAKE_CURRENT_SOURCE_DIR}/proto/json.proto)

    include_directories(proto)
    include_directories(${PROTOBUF_INCLUDE_DIR})
    include_directories(${gRPC_INCLUDE_DIR})
    set(REMOTE_SRCS
            src/remote_client.cpp src/remote_server.cpp
            src/handle_json.cpp src/handle_struct.cpp src/remote_type.cpp
            src/handle_net.cpp src/handle_grpc_client.cpp src/handle_grpc_server.cpp
            proto/json.grpc.pb.cc proto/json.pb.cc)
    add_library(remote_fun ${REMOTE_SRCS})
    target_link_libraries(remote_fun ${GRPC_STATIC_LIBRARY} ${PROTOBUF_LIBRARY} ${ZLIB_LIBRARY} pthread)

    add_executable(run_server test/run_server.cpp)
    add_executable(test_client1 test/test_client1.cpp)
    add_executable(test_client2 test/test_client2.cpp)
    target_link_libraries(run_server remote_fun) # ${PROTOBUF_LIBRARY} ${GRPC_STATIC_LIBRARY} ${ZLIB_LIBRARY} pthread)
    target_link_libraries(test_client1 remote_fun) # ${PROTOBUF_LIBRARY} ${GRPC_STATIC_LIBRARY} ${ZLIB_LIBRARY} pthread)
    target_link_libraries(test_client2 remote_fun) # ${PROTOBUF_LIBRARY} ${GRPC_STATIC_LIBRARY} ${ZLIB_LIBRARY} pthread)
endif ()


message("---------------------------- end MULTI_MASTER_ZHANG")
