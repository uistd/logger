#include "server.h"
#include "work.h"
#include "logger.h"
//进程ID文件路径
static const char *server_pid_file;

//子进程个数（暂时不支持多进程，但底层框架已经支持）
int server_process_num = 1;

//日志
yile_log_t console_log;

//显示帮助信息
static void show_help() {
    const char *help_string = "-----------------log server------------------\n"
            "\n"
            "-d   damond\n"
            "-c   config file\n"
            "Sample：./server -c config.ini\n"
            "------------------------------------------------------------\n";
    printf("%s", help_string);
}

int main(int argc, char *argv[]) {
    const char *config_file = NULL;
    int arg_c;
    int daemon_flag = 0;
    while (-1 != (arg_c = getopt(argc, argv, "c:d"))) {
        switch (arg_c) {
            case 'c':
                config_file = strdup(optarg);
                break;
            case 'd':
                daemon_flag = 1;
                break;
            default:
                show_help();
                return YILE_ERROR;
        }

    }
    //没有指定config_file
    if (NULL == config_file) {
        show_help();
        return YILE_ERROR;
    }
    const char *section = "SERVER";
    yile_ini_t ini_obj = {0};
    if (YILE_OK != yile_ini_parse(config_file, &ini_obj)) {
        return YILE_ERROR;
    }
    const char *log_path = yile_ini_get_string(section, "log_path", "/data/logs/", &ini_obj);
    //如果是守护进程, 要处理输出日志
    if (daemon_flag) {
        yile_process_daemon();
        yile_log_init(&console_log, log_path, section, LOG_SPLIT_BY_DAY);
    }

    //生成进程pid文件
    const char *tmp_pid_file = yile_ini_get_string(section, "pid_file", "/tmp/uis_log_server.pid", &ini_obj);
    if (YILE_OK != yile_process_pid_access(tmp_pid_file)) {
        return YILE_ERROR;
    }
    //日志级别
    int log_level = yile_ini_get_int(section, "log_level", 0xFF, &ini_obj);
    set_global_log_level(log_level);
    server_pid_file = strdup(tmp_pid_file);
    int pid = getpid();
    if (YILE_OK != yile_process_pid_touch(server_pid_file, pid)) {
        fprintf(stderr, "main() Write pid to file error\n");
        return -1;
    }
    int log_idle_time = yile_ini_get_int(section, "close_idle_file", 3600, &ini_obj);
    init_logger(log_path, 1, log_idle_time);

    //epoll初始化
    if (YILE_OK != work_main_event_set()) {
        return YILE_ERROR;
    }
    //server初始化
    if (YILE_OK != work_server_init(&ini_obj, section)) {
        return YILE_ERROR;
    }

    //初始化进程（暂时不支持多进程）
    yile_process_conf_t process_conf = {0};
    process_conf.child_num = server_process_num;
    yile_process_init(&process_conf);

    //清理ini
    yile_ini_clear(&ini_obj);
    //保存avc argv
    yile_process_save_arg(argc, argv);
    //进程开始工作
    yile_process_start_work();
    //正常退出，删除pid文件
    yile_process_pid_unlink(server_pid_file);
    return 0;
}