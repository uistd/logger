//
// Created by huangshunzhao on 2017/12/12.
//

#ifndef SERVER_LOG_H
#define SERVER_LOG_H

//日志级别
#define LOG_LEVEL_DEBUG 0x80
#define LOG_LEVEL_INFO 0x40
#define LOG_LEVEL_NOTICE 0x20
#define LOG_LEVEL_WARNING 0x10
#define LOG_LEVEL_ERROR 0x8
#define LOG_LEVEL_CRITICAL 0x4
#define LOG_LEVEL_ALERT 0x2
#define LOG_LEVEL_EMERGENCY 0x1

extern int global_log_level;

/**
 * 设置日志级别
 * @param log_level
 */
void set_global_log_level(int log_level);

#define log_debug(format, args...)if(global_log_level & LOG_LEVEL_DEBUG){printf( "[DEBUG]" format "\n", ##args );}
#define log_save(format, args...)printf( "[LOG]" format "\n", ##args )
#define log_info(format, args...)if(global_log_level & LOG_LEVEL_INFO){printf( "[INFO]" format "\n", ##args );}
#define log_notice(format, args...)if(global_log_level & LOG_LEVEL_NOTICE){printf( "[NOTICE]" format "\n", ##args );}
#define log_warning(format, args...)if(global_log_level & LOG_LEVEL_WARNING){printf( "[WARNING]" format "\n", ##args );}
#define log_error(format, args...)if(global_log_level & LOG_LEVEL_ERROR){printf( "[ERROR]" format "\n", ##args );}
#define log_critical(format, args...)if(global_log_level & LOG_LEVEL_CRITICAL){printf( "[CRITICAL]" format "\n", ##args );}
#define log_alert(format, args...)if(global_log_level & LOG_LEVEL_ALERT){printf( "[ALERT]" format "\n", ##args );}
#define log_emergency(format, args...)if(global_log_level & LOG_LEVEL_EMERGENCY){printf( "[EMERGENCY]" format "\n", ##args );}

#endif //SERVER_LOG_H
