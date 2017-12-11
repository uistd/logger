//以下代码由YGP https://code.aliyun.com/188332616/basexml 自动生成
#include "proto_action_dispatch.h"
/**
 * 路由
 * @param fd_info
 * @param read_buf
 * @return int
 */
int proto_action_dispatch(yile_connection_t *fd_info, yile_buf_t *read_buf) {
    packet_head_t *action_head = (packet_head_t*)read_buf->data;
    read_buf->read_pos = sizeof( packet_head_t );
    if (action_head->size != read_buf->write_pos - read_buf->read_pos) {
        yile_connection_close(fd_info);
        return YILE_ERROR;
    }
    int action_re;
    switch(action_head->pack_id) {
        case ACTION_ID_WRITE_LOG:
        {
            yile_protocol_stack_pool(proto_result, PROTO_SIZE_WRITE_LOG);
            uint32_t need_size = size_read_write_log(read_buf);
            if (need_size > PROTO_SIZE_WRITE_LOG) {
                yile_protocol_pool_resize(&proto_result, need_size);
            }
            proto_write_log_t *req_pack = read_write_log(read_buf, &proto_result);
            if (NULL == req_pack) {
                try_free_proto_pool(proto_result);
                yile_connection_close(fd_info);
                return YILE_ERROR;
            }
            #ifdef PROTOCOL_DEBUG
            printf("Action_id:100 Action: request_write_log\n");
            print_write_log(req_pack);
            #endif
            action_re = request_write_log(fd_info, req_pack);
            try_free_proto_pool(proto_result);
        }
        break;
        case ACTION_ID_TAIL:
        {
            yile_protocol_stack_pool(proto_result, PROTO_SIZE_TAIL);
            uint32_t need_size = size_read_tail(read_buf);
            if (need_size > PROTO_SIZE_TAIL) {
                yile_protocol_pool_resize(&proto_result, need_size);
            }
            proto_tail_t *req_pack = read_tail(read_buf, &proto_result);
            if (NULL == req_pack) {
                try_free_proto_pool(proto_result);
                yile_connection_close(fd_info);
                return YILE_ERROR;
            }
            #ifdef PROTOCOL_DEBUG
            printf("Action_id:301 Action: request_tail\n");
            print_tail(req_pack);
            #endif
            action_re = request_tail(fd_info, req_pack);
            try_free_proto_pool(proto_result);
        }
        break;
        default:
            action_re = YILE_ERROR;
    }
    if (YILE_OK != action_re) {
        yile_connection_close(fd_info);
        return YILE_ERROR;
    }
    return YILE_OK;
}
