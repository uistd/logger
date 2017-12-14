//
// Created by huangshunzhao on 2017/12/12.
//

#include <math.h>
#include "logger.h"
#include "map.h"

#define SPLIT_BY_HOUR 1
#define SPLIT_BY_DAY 2
#define LOG_PATH_BUFF 512

//空闲key
static uis_logger_t *free_logger_head;

//空闲key数量
static int free_item_num = 0;

//空闲key最大数量
const static int free_logger_max_num = 200;

//日志目录
static char *uis_log_path;

//日志分割方式
static int uis_split_type;

//最长空间时间
static int uis_max_idle_time;

/**
 * 所有用户认证列表
 */
ffan_map_t logger_map;

/**
 * 生成新的uis_logger_t
 */
uis_logger_t *uis_logger_new() {
    uis_logger_t *tmp;
    if (free_item_num > 0) {
        tmp = free_logger_head;
        free_logger_head = free_logger_head->next;
        --free_item_num;
    } else {
        tmp = (uis_logger_t *) malloc(sizeof(uis_logger_t));
        if (NULL == tmp) {
            fprintf(stderr, "role_info_new() out of memory\n");
            exit(1);
        }
    }
    memset(tmp, 0, sizeof(uis_logger_t));
    return tmp;
}

/**
 * 释放
 * @param logger
 */
void uis_logger_free(uis_logger_t *logger) {
    log_debug("Free logger file_name:%s", logger->file_name);
    if (logger->file_handle) {
        fclose(logger->file_handle);
    }
    if (NULL != logger->file_name) {
        free(logger->file_name);
    }
    if (free_item_num >= free_logger_max_num) {
        free(logger);
    } else {
        logger->next = free_logger_head;
        free_logger_head = logger;
        ++free_item_num;
    }
}

/**
 * 初始化
 * @param logger_path
 * @param split_type 分割方式 1: 按小时分割 2:按天分割
 * @param max_idle_time 最长的空闲时间, 超过时间, 将关闭日志文件
 * @return
 */
int init_logger(const char* logger_path, int split_type, int max_idle_time) {
    uis_max_idle_time = max_idle_time;
    if ('/' != logger_path[0]) {
        log_error("日志目录必须是绝对路径");
        return YILE_ERROR;
    }
    size_t len = strlen(logger_path);
    char *div_char = "";
    if ('/' != logger_path[len - 1]) {
        div_char = "/";
    }
    char path_name_buf[LOG_PATH_BUFF + 1];
    snprintf(path_name_buf, LOG_PATH_BUFF, "%s%s", logger_path, div_char);
    uis_log_path = strdup(path_name_buf);
    uis_split_type = split_type;
    map_init(&logger_map, 100);
    return YILE_OK;
}

/**
 * 返回下一次分割时间
 * @param now_time
 * @return long
 */
long uis_next_split_time(long now_time) {
    uint32_t split_value;
    if (SPLIT_BY_HOUR == uis_split_type) {
        split_value = 3600;
    } else {
        split_value = 86400;
    }
    return split_value * ((now_time / split_value) + 1);
}

/**
 * 自动创建目录
 * @param path_name
 * @return
 */
static int uis_log_create_dir(const char *path_name) {
    char path_pool[LOG_PATH_BUFF];
    char *tmp_path = path_pool;
    size_t len = strlen(path_name);
    memcpy(tmp_path, path_name, len + 1);
    int i, re = YILE_OK;
    for (i = 1; i <= len; i++) {
        if ('/' == path_name[i]) {
            tmp_path[i] = '\0';
            if (0 != access(tmp_path, F_OK)) {
                if (0 != mkdir(tmp_path, 0755)) {
                    re = YILE_ERROR;
                    break;
                }
            }
            tmp_path[i] = '/';
        }
    }
    return re;
}

/**
 * 检查日志文件
 * @param logger
 */
int logger_file_check(uis_logger_t *logger) {
    time_t now_time = time(0);
    logger->active_time = now_time;
    if (logger->file_handle > 0 && now_time < logger->split_time) {
        return YILE_OK;
    }
    //如果已经打开文件
    if (logger->file_handle > 0) {
        fclose(logger->file_handle);
    }
    char date_str[12];
    struct tm *t = localtime(&now_time);
    snprintf(date_str, sizeof(date_str) - 1, "%d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);    //年月日
    char file_name_pool[LOG_PATH_BUFF + 1];
    if (uis_split_type == SPLIT_BY_HOUR) {
        char hour[5];
        snprintf(hour, sizeof(hour) - 1, "%02d", t->tm_hour);
        snprintf(file_name_pool, LOG_PATH_BUFF, "%s.%s%s.log", logger->file_name, date_str, hour);
    } else {
        snprintf(file_name_pool, LOG_PATH_BUFF, "%s.%s.log", logger->file_name, date_str);
    }
    if (YILE_OK != uis_log_create_dir(file_name_pool)) {
        return YILE_ERROR;
    }
    logger->split_time = uis_next_split_time(now_time);
    logger->file_handle = fopen(file_name_pool, "a+");
    if (NULL == logger->file_handle) {
        log_error("Can not open file:%s", file_name_pool);
        return YILE_ERROR;
    }
    return YILE_OK;
}

/**
 * 写日志
 * @param file_name
 * @param content
 * @param length
 */
void write_log(const char *file_name, const char *content, size_t length) {

    size_t key_len = strlen(file_name) + 1;
    uis_logger_t *logger = (uis_logger_t *) map_get(&logger_map, file_name, key_len);
    if (NULL == logger) {
        logger = uis_logger_new();
        char path_name_buf[LOG_PATH_BUFF + 1];
        const char *tmp_file_name = file_name;
        if ('/' == file_name[0]) {
            tmp_file_name = &file_name[0];
        }
        //文件名
        snprintf(path_name_buf, LOG_PATH_BUFF, "%s%s", uis_log_path, tmp_file_name);
        logger->file_name = strdup(path_name_buf);
        //将新logger加入map
        map_set(&logger_map, file_name, key_len, logger);
    }
    if (YILE_OK != logger_file_check(logger)) {
        return;
    }
    size_t write_re = fwrite(content, 1, length, logger->file_handle);
    if (write_re != length) {
        log_error("write log size error");
    } else {
        fflush(logger->file_handle);
    }
}


/**
 * 删除长时间没有心心跳的用户
 * @param key
 * @param key_len
 * @param role
 */
void logger_idle_check(const char *key, size_t key_len, void *tmp) {
    uis_logger_t *logger = (uis_logger_t *) tmp;
    time_t now_time = time(0);
    if (now_time - logger->active_time > uis_max_idle_time) {
        log_debug("Close idle logger: %s", logger->file_name);
        //从map中移除
        map_unset(&logger_map, key, key_len);
        uis_logger_free(logger);
    }
}

/**
 * 清理打开的空闲日志
 * @param tmp
 * @return
 */
int clean_idle_logger(yile_connection_t *tmp)
{
    map_walk(&logger_map, logger_idle_check);
    return YILE_OK;
}
