//
// Created by huangshunzhao on 2017/12/12.
//

#ifndef SERVER_ACTION_H
#define SERVER_ACTION_H

#include "common.h"
#include "protocol/def.h"
#include "protocol/proto.h"

#define DEFAULT_RESPONSE_BUF_SIZE 10240
//发送数据缓存区
extern char DEFAULT_RESPONSE_BUF[DEFAULT_RESPONSE_BUF_SIZE];
#define response_buf_init(buf_name) {0}; \
    char DEFAULT_RESPONSE_BUF[DEFAULT_RESPONSE_BUF_SIZE]; \
    buf_name.is_stack = 1; \
    buf_name.data = DEFAULT_RESPONSE_BUF; \
    buf_name.is_data_stack = 1; \
    buf_name.max_pos = DEFAULT_RESPONSE_BUF_SIZE

/**
 * action路由
 * @param fd_info
 * @param read_buf
 * @return
 */
int action_dispatch(yile_connection_t *fd_info, yile_buf_t *read_buf, uint32_t hash_id);

/**
 * 发送数据
 * @param fd_info
 * @param buf
 */
void send_response_protocol(yile_connection_t *fd_info, yile_buf_t *buf);

#endif //SERVER_ACTION_H
