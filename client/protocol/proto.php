<?php
//以下代码由YGP https://code.aliyun.com/188332616/basexml 自动生成

/**
 * 打包数据 写日志
 * @param array $data 数据
 * @return string
 */
function proto_pack_write_log( $data )
{
    $bin_str = '';
    $vfile_len = strlen($data['file']);
    $vcontent_len = strlen($data['content']);
    $bin_str .= pack('Sa'.$vfile_len.'Sa'.$vcontent_len.'', $vfile_len, $data['file'], $vcontent_len, $data['content']);
    $head_str = pack("LS", strlen( $bin_str ), 100);
    $bin_str = $head_str . $bin_str;
    return $bin_str;
}

