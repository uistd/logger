//
// Created by huangshunzhao on 2017/12/12.
//

#ifndef SERVER_LOGGER_H
#define SERVER_LOGGER_H
#include "common.h"

typedef struct uis_logger_s uis_logger_t;
struct uis_logger_s{
    FILE			*file_handle;		//打开的文件
    time_t			active_time;		//活跃时间
    time_t			split_time;		    //日志分割时间
    char*			file_name;			//文件名
    struct uis_logger_s *next;          //用于链表
};

/**
 * 写日志
 * @param file_name
 * @param content
 * @param length
 */
void write_log(const char *file_name, const char *content, size_t length);

/**
 * 初始化
 * @param logger_path
 * @param split_type 分割方式 1: 按小时分割 2:按天分割
 * @param max_idle_time 最长的空闲时间, 超过时间, 将关闭日志文件
 * @return
 */
int init_logger(const char* logger_path, int split_type, int max_idle_time);

/**
 * 清理打开的空闲日志
 * @param tmp
 * @return
 */
int clean_idle_logger(yile_connection_t *tmp);

#endif //SERVER_LOGGER_H
