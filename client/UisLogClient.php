<?php

namespace FFan\Uis\Log;

use FFan\Std\Common\Utils;
use FFan\Std\Logger\LoggerBase;
use FFan\Std\Logger\LogHelper;
use FFan\Std\Logger\LogLevel;

/**
 * Class UisLogClient
 * @package FFan\Uis\Log
 */
class UisLogClient extends LoggerBase
{
    /**
     * 是否记录请求头
     */
    const OPT_LOG_HEADER = 1;

    /**
     * 是否将日志写入buffer，然后一次性写入文件
     */
    const OPT_WRITE_BUFFER = 2;

    /**
     * 是否记录日志类型
     */
    const OPT_LOG_TYPE_STR = 4;

    /**
     * 每一条日志换行
     */
    const OPT_BREAK_EACH_LOG = 8;

    /**
     * 第一个请求换一次行
     */
    const OPT_BREAK_EACH_REQUEST = 16;

    /**
     * @var int 日志级别
     */
    private $log_level;

    /**
     * @var string 文件名
     */
    private $file_name;

    /**
     * @var array
     */
    private $msg_buffer;

    /**
     * @var bool
     */
    private $is_first_log = true;

    /**
     * @var int 选项
     */
    private $current_opt;

    /**
     * @var string
     */
    private $break_flag;

    /**
     * @var bool
     */
    private $is_write_buffer;

    /**
     * UisLogClient constructor.
     * @param string $path 目录
     * @param string $file_name 文件名
     * @param int $log_level
     * @param int $option
     */
    public function __construct($path, $file_name = 'log', $log_level = 0, $option = 0)
    {
        parent::__construct();
        if ($log_level <= 0) {
            $log_level = 0xffff;
        }
        $this->file_name = Utils::joinFilePath($path, $file_name);
        $this->log_level = $log_level;
        if (0 === $option) {
            //默认参数
            $option = self::OPT_LOG_HEADER | self::OPT_BREAK_EACH_LOG;
            $option |= self::OPT_WRITE_BUFFER;
        }
        $this->setOption($option);
    }

    /**
     * 设置option
     * @param int $append_option
     * @param int $remove_option
     * @internal param int $option
     */
    public function setOption($append_option = 0, $remove_option = 0)
    {
        if ($append_option > 0) {
            $this->current_opt |= $append_option;
        }
        if ($remove_option > 0) {
            $this->current_opt ^= $remove_option;
        }
        $this->parseOption();
    }

    /**
     * 解析设置
     */
    private function parseOption()
    {
        $opt = $this->current_opt;
        $this->is_write_buffer = ($opt & self::OPT_WRITE_BUFFER) > 0;
        $this->split_name = null;
        if (($opt & self::OPT_BREAK_EACH_REQUEST) > 0) {
            $this->break_flag = ' | ';
        } else {
            $this->break_flag = PHP_EOL;
        }
    }

    /**
     * @return resource
     */
    private function getLogHandler()
    {

    }

    /**
     * 收到日志
     * @param int $log_level
     * @param string $content
     */
    public function onLog($log_level, $content)
    {
        if (!($this->log_level & $log_level)) {
            return;
        }
        $file_handle = $this->getLogHandler();
        if (null === $file_handle) {
            $this->is_disable = true;
            return;
        }
        //前面增加的内容
        $prefix_str = '';
        //增加类型
        if (($this->current_opt & self::OPT_LOG_TYPE_STR) > 0) {
            $prefix_str = '[' . LogLevel::levelName($log_level) . ']';
        }

        //如果 每次请求内不换行 把换行符替换成
        if (($this->current_opt & self::OPT_BREAK_EACH_REQUEST) > 0) {
            $content = str_replace(PHP_EOL, '\\n', $content);
        }

        //第一条日志强制换行
        if ($this->is_first_log) {
            if (($this->current_opt & self::OPT_LOG_HEADER) > 0) {
                $prefix_str .= LogHelper::logHeader();
            }
            $time_str = '[' . strftime('%H:%M:%S') . ']';
            $this->is_first_log = false;
            $prefix_str = PHP_EOL . $time_str . $this->break_flag . $prefix_str;
        }

        if (!empty($prefix_str)) {
            $content = $prefix_str . $content;
        }
        if ($this->is_write_buffer) {
            $this->msg_buffer[] = $content;
        } else {
            $write_len = fwrite($file_handle, $content . $this->break_flag);
            if (false === $write_len) {
                $this->is_disable = true;
            }
        }
    }

}
