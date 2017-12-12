cmake_minimum_required(VERSION 2.8)
project(server)
add_definitions(-g)
set(SOURCE_FILES
        server.c server.h common.h
        protocol/proto.c protocol/proto.h
        protocol/proto_action_dispatch.c protocol/proto_action_dispatch.h logger.c logger.h log.c log.h map.c map.h work.c work.h action.c action.h)
INCLUDE_DIRECTORIES(
        yile
        yile/yile_log/include
        yile/yile_buf/include
        yile/yile_ini/include
        yile/yile_process/include
        yile/yile_protocol/include
        yile/yile_net/include
        protocol/
)
LINK_DIRECTORIES(
        yile/yile_buf/lib
        yile/yile_process/lib
        yile/yile_ini/lib
        yile/yile_protocol/lib
        yile/yile_log/lib
        yile/yile_net/lib
)
LINK_LIBRARIES(
        yileini_debug
        yileprocess_debug
        yilelog_debug
        yilebuf_debug
        yileprotocol_debug
        yilenet_debug
)
add_executable(server ${SOURCE_FILES})