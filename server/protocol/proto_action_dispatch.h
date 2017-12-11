//以下代码由YGP https://code.aliyun.com/188332616/basexml 自动生成
#ifndef YILE_PROTOCOL_ACTION_DISPATCH_H
#define YILE_PROTOCOL_ACTION_DISPATCH_H
#include "def.h"
#include "proto.h"
/**
 * 路由
 * @param fd_info
 * @param read_buf
 * @return int
 */
int proto_action_dispatch(yile_connection_t *fd_info, yile_buf_t *read_buf);

/**
 * Action id: 100 写日志
 * @param fd_info
 * @param req_pack
 * @return int
 */
int request_write_log(yile_connection_t *fd_info, proto_write_log_t *req_pack);

/**
 * Action id: 301 tail
 * @param fd_info
 * @param req_pack
 * @return int
 */
int request_tail(yile_connection_t *fd_info, proto_tail_t *req_pack);
#endif
