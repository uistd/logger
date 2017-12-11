//以下代码由YGP https://code.aliyun.com/188332616/basexml 自动生成
#ifndef YILE_PROTOCOL_PROTO_H
#define YILE_PROTOCOL_PROTO_H
#include <stdint.h>
#include <stdlib.h>
#include "yile_protocol.h"
#if defined __cplusplus
extern "C" {
#endif
#pragma pack(1)

typedef struct proto_write_log_t proto_write_log_t;

typedef struct proto_tail_t proto_tail_t;
//写日志
struct proto_write_log_t{
	/** 日志文件 */
	char*												file;
	/** 内容 */
	char*												content;
};
//tail
struct proto_tail_t{
	/** 文件名 */
	char*												file;
};
#pragma pack()

/**
 * 解析 写日志
 */
proto_write_log_t *read_write_log( yile_buf_t *byte_pack, protocol_pool_t *result_pool );

/**
 * 解析 tail
 */
proto_tail_t *read_tail( yile_buf_t *byte_pack, protocol_pool_t *result_pool );
/**
 * 计算解析内存 写日志
 */
uint32_t size_read_write_log( yile_buf_t *byte_pack );
/**
 * 计算解析内存 tail
 */
uint32_t size_read_tail( yile_buf_t *byte_pack );
#ifdef PROTOCOL_DEBUG

/**
 * 打印 写日志
 */
void print_write_log( proto_write_log_t *re );

/**
 * 打印 tail
 */
void print_tail( proto_tail_t *re );
#endif
#if defined __cplusplus
}
#endif
#endif
