//
// Created by huangshunzhao on 2017/12/12.
//

#ifndef SERVER_MAP_H
#define SERVER_MAP_H

#include "common.h"

#define MIN_MAP_HASH_TABLE 8
#define MAP_RETURN_FAIL 1
#define MAP_RETURN_SUCCESS 0

//关联数组的每一项
typedef struct map_item_s map_item_t;
struct map_item_s {
    uint32_t hash;          //key的hash值
    const char *key;        //key
    size_t key_len;         //key长度（为了二进制安全）
    void *value;        //数据
    map_item_t *next;   //用于链表
};
//关联数组
typedef struct ffan_map_s ffan_map_t;
struct ffan_map_s {
    size_t table_size;     //数据大小
    size_t table_mask;     //hash桶大小
    int size;           //当前大小
    map_item_t **hash_table; //hash表头
};

//遍历回调
typedef void (*map_walk_cb_pt)(const char *key, size_t key_len, void *value);

/**
 * 初始化一个关联数组
 * @param arr
 * @param table_size
 */
int map_init(ffan_map_t *arr, size_t table_size);

/**
 * 关联数组设置一个key，如果key存在将替换
 * @param arr
 * @param key
 * @param key_len
 * @param value
 * @return size_t
 */
size_t map_set(ffan_map_t *arr, const char *key, size_t key_len, void *value);


/**
 * 关联数组增加一个key，如果key存在将失败
 * @param arr
 * @param key
 * @param key_len
 * @param value
 * @return size_t
 */
size_t map_add(ffan_map_t *arr, const char *key, size_t key_len, void *value);

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
 * 获取关联数组的值
 * @param arr
 * @param key
 * @param key_len
 * @return
 */
void *map_get(ffan_map_t *arr, const char *key, size_t key_len);

/**
 * 简单的遍历方法，无序的
 */
void map_walk(ffan_map_t *arr, map_walk_cb_pt cb);

#endif //SERVER_MAP_H
