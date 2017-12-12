//
// Created by huangshunzhao on 2017/12/12.
//

#include "action.h"
#include "protocol/proto_action_dispatch.h"
#include "logger.h"

/**
 * action路由
 * @param fd_info
 * @param read_buf
 * @return
 */
int action_dispatch(yile_connection_t *fd_info, yile_buf_t *read_buf) {
    return proto_action_dispatch(fd_info, read_buf);
}

/**
 * Action id: 100 写日志
 * @param fd_info
 * @param req_pack
 * @return int
 */
int request_write_log(yile_connection_t *fd_info, proto_write_log_t *req_pack)
{
    write_log(req_pack->file, req_pack->content, strlen(req_pack->content) + 1);
}

/**
 * Action id: 301 tail
 * @param fd_info
 * @param req_pack
 * @return int
 */
int request_tail(yile_connection_t *fd_info, proto_tail_t *req_pack)
{

}