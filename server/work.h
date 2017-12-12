//
// Created by huangshunzhao on 2017/12/12.
//

#ifndef SERVER_WORK_H
#define SERVER_WORK_H

#include "common.h"

/**
 * server 初始化
 */
int work_server_init(yile_ini_t *ini_obj, const char *section);

/**
 * 主进程的epoll设置
 */
int work_main_event_set();

#endif //SERVER_WORK_H
