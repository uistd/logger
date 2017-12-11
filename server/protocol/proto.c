//以下代码由YGP https://code.aliyun.com/188332616/basexml 自动生成
#include "proto.h"

/**
 * 解析 写日志
 */
proto_write_log_t *read_write_log( yile_buf_t *byte_pack, protocol_pool_t *result_pool )
{
    proto_write_log_t *re_struct = NULL;
    if ( NULL == re_struct )
    {
        re_struct = ( proto_write_log_t* )yile_protocol_malloc( result_pool, sizeof( proto_write_log_t ) );
        if ( NULL == re_struct )
        {
            return re_struct;
        }
    }
    re_struct->file = yile_protocol_read_string( byte_pack, result_pool );
    re_struct->content = yile_protocol_read_string( byte_pack, result_pool );
    if ( byte_pack->error_code || result_pool->error_code )
    {
        return NULL;
    }
    return re_struct;
}

/**
 * 解析 tail
 */
proto_tail_t *read_tail( yile_buf_t *byte_pack, protocol_pool_t *result_pool )
{
    proto_tail_t *re_struct = NULL;
    if ( NULL == re_struct )
    {
        re_struct = ( proto_tail_t* )yile_protocol_malloc( result_pool, sizeof( proto_tail_t ) );
        if ( NULL == re_struct )
        {
            return re_struct;
        }
    }
    re_struct->file = yile_protocol_read_string( byte_pack, result_pool );
    if ( byte_pack->error_code || result_pool->error_code )
    {
        return NULL;
    }
    return re_struct;
}
/**
 * 计算解析内存 写日志
 */
uint32_t size_read_write_log( yile_buf_t *byte_pack )
{
    uint32_t old_byte_pack_size = byte_pack->read_pos;
    protocol_pool_t unpack_pool = {0};
    protocol_pool_t *result_pool = &unpack_pool;
    yile_protocol_premalloc( result_pool, sizeof( proto_write_log_t ) );
    yile_protocol_str_len_t tmp_str_len;
    tmp_str_len = yile_protocol_read_ushort( byte_pack );
    yile_protocol_size_check( byte_pack, tmp_str_len );
    yile_protocol_premalloc( result_pool, tmp_str_len + 1 );
    tmp_str_len = yile_protocol_read_ushort( byte_pack );
    yile_protocol_size_check( byte_pack, tmp_str_len );
    yile_protocol_premalloc( result_pool, tmp_str_len + 1 );
    byte_pack->read_pos = old_byte_pack_size;
    if ( result_pool->error_code || byte_pack->error_code )
    {
        return 0;
    }
    return result_pool->max_size;
}
/**
 * 计算解析内存 tail
 */
uint32_t size_read_tail( yile_buf_t *byte_pack )
{
    uint32_t old_byte_pack_size = byte_pack->read_pos;
    protocol_pool_t unpack_pool = {0};
    protocol_pool_t *result_pool = &unpack_pool;
    yile_protocol_premalloc( result_pool, sizeof( proto_tail_t ) );
    yile_protocol_str_len_t tmp_str_len;
    tmp_str_len = yile_protocol_read_ushort( byte_pack );
    yile_protocol_size_check( byte_pack, tmp_str_len );
    yile_protocol_premalloc( result_pool, tmp_str_len + 1 );
    byte_pack->read_pos = old_byte_pack_size;
    if ( result_pool->error_code || byte_pack->error_code )
    {
        return 0;
    }
    return result_pool->max_size;
}
#ifdef PROTOCOL_DEBUG

/**
 * 打印 写日志
 */
void print_write_log( proto_write_log_t *re )
{
    int rank = 0;
    char prefix_char[ MAX_LIST_RECURSION * 4 + 1 ];
    yile_printf_tab_string( prefix_char, rank );
    printf( "write_log\n" );
    printf( "%s(\n", prefix_char );
    printf( "    %s[file] = > ", prefix_char );
    printf( "%s\n", re->file );
    printf( "    %s[content] = > ", prefix_char );
    printf( "%s\n", re->content );
    printf( "%s)\n", prefix_char );
}

/**
 * 打印 tail
 */
void print_tail( proto_tail_t *re )
{
    int rank = 0;
    char prefix_char[ MAX_LIST_RECURSION * 4 + 1 ];
    yile_printf_tab_string( prefix_char, rank );
    printf( "tail\n" );
    printf( "%s(\n", prefix_char );
    printf( "    %s[file] = > ", prefix_char );
    printf( "%s\n", re->file );
    printf( "%s)\n", prefix_char );
}
#endif
