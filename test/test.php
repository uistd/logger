<?php

require_once '../client/protocol/proto.php';
$fd = $socket = stream_socket_client('tcp://127.0.0.1:10777', $err_no, $err_msg, 3);
if (!$fd) {
    echo "{$err_msg} ({$err_no})<br />\n";
    die;
}

$log_arr = array(
    'file' => 'feed/channel/food',
    'content' => 'This is test' . PHP_EOL
);

$bin_str = proto_pack_write_log($log_arr);

fwrite($fd, $bin_str);

sleep(2);