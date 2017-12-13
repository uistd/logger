//
// Created by huangshunzhao on 2017/12/12.
//

#include "log.h"

//全局日志级别
int global_log_level = LOG_LEVEL_WARNING;

/**
 * 设置日志级别
 * @param log_level
 */
void set_global_log_level(int log_level) {
    global_log_level = log_level;
}
