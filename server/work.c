//
// Created by huangshunzhao on 2017/12/12.
//

#include "work.h"
#include "action.h"
#include "logger.h"

/**
 * 数据到达
 */
static int work_server_request(yile_connection_t *fd_info) {
    log_debug("Log server on data:%d", fd_info->fd);
    return action_dispatch(fd_info, fd_info->read_buf);
}

/**
 * 关闭回调
 * @return
 */
static int work_server_close(yile_connection_t *fd_info) {
    if (fd_info->write_buf) {
        log_notice("fd: %d has data %u byte to send ", fd_info->fd, yile_buf_read_available(fd_info->write_buf));
    }
    log_debug("fd:%d close", fd_info->fd);
    return YILE_OK;
}

/**
 * 新来连接
 */
static int work_server_new_conn(yile_connection_t *tmp) {
    //数据响应
    yile_event_handle_t server_data_handle = {work_server_request, work_server_close};
    log_debug("new connect:%d", tmp->fd);
    yile_connection_set_action(tmp, &server_data_handle);
    return YILE_OK;
}

/**
 * 主进程的epoll设置
 */
int work_main_event_set() {
    yile_event_conf_t server_event_conf = {0};
    yile_epoll_conf_t server_epoll_conf;
    //使用 边缘触发 模式
    server_epoll_conf.et_trigger = 1;
    server_epoll_conf.max_event = 256;
    yile_epoll_init(&server_epoll_conf, &server_event_conf);
    return YILE_OK;
}

/**
 * server 初始化
 */
int work_server_init(yile_ini_t *ini_obj, const char *section) {
    const char *server_host = yile_ini_get_string(section, "server_host", "127.0.0.1", ini_obj);
    int server_port = yile_ini_get_int(section, "server_port", -1, ini_obj);
    if (-1 == server_port) {
        fprintf(stderr, "Server port read failed!\n");
        return YILE_ERROR;
    }
    //局域网
    yile_connection_t *server_listen_fd = yile_listen_fd(server_host, server_port);
    if (NULL == server_listen_fd) {
        fprintf(stderr, "Can not listen %s:%d\n", server_host, server_port);
        return YILE_ERROR;
    }
    //局域网通信处理
    yile_event_handle_t server_listen_handle = {work_server_new_conn, NULL};
    //设置事件
    yile_connection_set_action(server_listen_fd, &server_listen_handle);
    //设置一个定时器，关闭一些打开了, 但不活跃的日志文件
    yile_connection_t *timer_fd = yile_timer_fd();
    //每2分钟定时间踢掉没心跳用户
    yile_set_interval(timer_fd, 120000, clean_idle_logger);
    return YILE_OK;
}