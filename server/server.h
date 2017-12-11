//
// Created by bluebird on 2016-9-14.
//

#ifndef UIS_SERVER_H
#define UIS_SERVER_H

#include "common.h"

#define MIN_MAP_HASH_TABLE 8
#define MAP_RETURN_FAIL 1
#define MAP_RETURN_SUCCESS 0

//数组的每一项
typedef struct map_item_s map_item_t;
struct map_item_s {
    ulong hash;        //key的hash值
    const char *key;        //key
    size_t key_len;    //key长度（为了二进制安全）
    void *value;        //数据
    map_item_t *next;        //用于链表
};
//数组
typedef struct ffan_map_s ffan_map_t;
struct ffan_map_s {
    int table_size; //数据大小
    int table_mask;    //hash桶大小
    int size;        //当前大小
    map_item_t **hash_table; //hash表头
    ffan_map_t *next;        //用于链表
};

/**
 * 初始化一个数组
 * @param arr
 * @param table_size
 */
int map_init(ffan_map_t *arr, size_t table_size);

/**
 * 数组设置一个key，如果key存在将替换
 * @param arr
 * @param key
 * @param key_len
 * @param value
 * @return int
 */
int map_set(ffan_map_t *arr, const char *key, size_t key_len, void *value);


/**
 * 数组增加一个key，如果key存在将失败
 * @param arr
 * @param key
 * @param key_len
 * @param value
 * @return int
 */
int map_add(ffan_map_t *arr, const char *key, size_t key_len, void *value);

/**
 * 在数据中查找一个值是否存在
 * @param arr
 * @param key
 * @param key_len
 * @return map_item_t
 */
map_item_t *map_exist(ffan_map_t *arr, const char *key, size_t key_len);

/**
 * 移除一个key
 * @param arr
 * @param key
 * @param key_len
 */
void map_unset(ffan_map_t *arr, const char *key, size_t key_len);

/**
 * 获取数组的值
 * @param arr
 * @param key
 * @param key_len
 * @return
 */
void *map_get(ffan_map_t *arr, const char *key, size_t key_len);

#endif //UIS_SERVER_H
