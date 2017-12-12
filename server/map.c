//
// Created by huangshunzhao on 2017/12/12.
//

#include "map.h"
#include "common.h"

#define map_is_same_key(tmp, item) tmp->hash == item->hash && tmp->key_len == item->key_len && 0 == strncmp(item->key, tmp->key, item->key_len)

//空闲key
static map_item_t *free_item_head;

//空闲key数量
static int free_item_num = 0;

//空闲key最大数量
const static int free_item_max_num = 200;

/**
 * 新建一个key
 */
static inline map_item_t *map_item_new(const char *key, size_t key_len) {
    map_item_t *tmp;
    if (free_item_num > 0) {
        tmp = free_item_head;
        free_item_head = free_item_head->next;
        --free_item_num;
    } else {
        tmp = (map_item_t *) malloc(sizeof(map_item_t));
        if (NULL == tmp) {
            fprintf(stderr, "map_item_new() out of memory\n");
            exit(1);
        }
    }
    //二进制的key也是安全的
    tmp->key = strndup(key, key_len);
    if (NULL == tmp->key) {
        log_emergency("Out of memory at: %s %d", __FILE__, __LINE__);
        exit(1);
    }
    tmp->key_len = key_len;
    return tmp;
}

/**
 * 释放一个key
 */
static inline void map_item_free(map_item_t *tmp) {
    if (NULL != tmp->key) {
        free((void *) tmp->key);
    }
    if (free_item_num >= free_item_max_num) {
        free(tmp);
    } else {
        tmp->next = free_item_head;
        free_item_head = tmp;
        ++free_item_num;
    }
}

/**
 * 复制一个Key
 * @return may_item_t
 */
static inline map_item_t *map_item_dup(map_item_t *item) {
    map_item_t *new_item = map_item_new(item->key, item->key_len);
    new_item->value = item->value;
    new_item->hash = item->hash;
    new_item->next = NULL;
    return new_item;
}

/**
 * 将数组的key转换成int，用于hash
 * 这个算法摘抄自 PHP 源码
 * @param arr_key
 * @param key_len
 * @return
 */
static inline uint32_t map_key_hash(const char *arr_key, size_t key_len) {
    register uint32_t hash = 5381;
    for (; key_len >= 8; key_len -= 8) {
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
        hash = ((hash << 5) + hash) + *arr_key++;
    }
    switch (key_len) {
        case 7:
            hash = ((hash << 5) + hash) + *arr_key++; /* fallthrough... */
        case 6:
            hash = ((hash << 5) + hash) + *arr_key++; /* fallthrough... */
        case 5:
            hash = ((hash << 5) + hash) + *arr_key++; /* fallthrough... */
        case 4:
            hash = ((hash << 5) + hash) + *arr_key++; /* fallthrough... */
        case 3:
            hash = ((hash << 5) + hash) + *arr_key++; /* fallthrough... */
        case 2:
            hash = ((hash << 5) + hash) + *arr_key++; /* fallthrough... */
        case 1:
            hash = ((hash << 5) + hash) + *arr_key++;
            break;
        case 0:
            break;
    }
    return hash;
}

/**
 * 是否存在某个key
 * @param arr
 * @param item
 * @return may_item_t
 */
static inline map_item_t *map_exist_by_item(ffan_map_t *arr, map_item_t *item) {
    size_t index = item->hash & arr->table_mask;
    if (NULL == arr->hash_table[index]) {
        return NULL;
    }
    map_item_t *tmp = arr->hash_table[index];
    while (NULL != tmp) {
        //hash是从小到大排列的，如果hash不相等，没有必要再检查下去了
        if (tmp->hash > item->hash) {
            break;
        }
        if (map_is_same_key(tmp, item)) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

/**
 * 初始化一个数组
 * @param arr
 * @param table_size
 */
int map_init(ffan_map_t *arr, size_t table_size) {
    //防止多次初始化
    if (NULL != arr->hash_table) {
        return YILE_ERROR;
    }
    if (0 == table_size) {
        table_size = MIN_MAP_HASH_TABLE;
    }
    arr->table_size = table_size;
    arr->table_mask = arr->table_size - 1;
    uis_malloc(arr->hash_table, sizeof(map_item_t *) * table_size);
    memset(arr->hash_table, 0, sizeof(map_item_t *) * table_size);
    arr->size = 0;
    return YILE_OK;
}

/**
 * 移除一个key
 * @param arr
 * @param item
 */
void map_unset_by_item(ffan_map_t *arr, map_item_t *item) {
    size_t index = item->hash & arr->table_mask;
    if (NULL == arr->hash_table[index]) {
        return;
    }
    map_item_t *tmp = arr->hash_table[index];
    map_item_t *last = NULL;
    while (NULL != tmp) {
        if (tmp->hash > item->hash) {
            break;
        }
        if (map_is_same_key(tmp, item)) {
            //表头
            if (NULL == last) {
                arr->hash_table[index] = tmp->next;
            } else {
                last->next = tmp->next;
            }
            map_item_free(tmp);
            --arr->size;
            break;
        }
        last = tmp;
        tmp = tmp->next;
    }
}

/**
 * 数组增加一个Key
 * @param arr
 * @param item
 * @param is_replace 如果key存在，是否替换
 */
static inline size_t map_set_by_item(ffan_map_t *arr, map_item_t *item, int is_replace) {
    if (map_exist_by_item(arr, item)) {
        if (is_replace) {
            map_unset_by_item(arr, item);
        } else {
            return MAP_RETURN_FAIL;
        }
    }
    size_t index = item->hash & arr->table_mask;
    map_item_t *new_item = map_item_dup(item);
    if (NULL == arr->hash_table[index]) {
        arr->hash_table[index] = new_item;
    } else {
        //如果已经存在，按hash从小到大的顺序排序
        map_item_t *tmp = arr->hash_table[index];
        map_item_t *last = NULL;
        while (NULL != tmp) {
            if (tmp->hash > new_item->hash) {
                if (NULL == last) {
                    arr->hash_table[index] = new_item;
                } else {
                    last->next = new_item;
                }
                new_item->next = tmp;
                break;
            }
            last = tmp;
            //到最后了
            if (NULL == tmp->next) {
                tmp->next = new_item;
                break;
            }
            tmp = tmp->next;
        }
    }
    ++arr->size;
    //@todo resize map
    return MAP_RETURN_SUCCESS;
}

/**
 * 数组增加一个key
 * @param arr
 * @param key
 * @param key_len
 * @param value
 * @return size_t
 */
size_t map_set(ffan_map_t *arr, const char *key, size_t key_len, void *value) {
    map_item_t arr_item;
    arr_item.hash = map_key_hash(key, key_len);
    arr_item.next = NULL;
    arr_item.key = key;
    arr_item.key_len = key_len;
    arr_item.value = value;
    return map_set_by_item(arr, &arr_item, 1);
}

/**
 * 数组增加一个key，如果key存在将失败
 * @param arr
 * @param key
 * @param key_len
 * @param value
 * @return size_t
 */
size_t map_add(ffan_map_t *arr, const char *key, size_t key_len, void *value) {
    map_item_t arr_item;
    arr_item.hash = map_key_hash(key, key_len);
    arr_item.next = NULL;
    arr_item.key = key;
    arr_item.key_len = key_len;
    arr_item.value = value;
    return map_set_by_item(arr, &arr_item, 0);
}

/**
 * 在数据中查找一个值是否存在
 * @param arr
 * @param key
 * @param key_len
 * @return may_item_t
 */
map_item_t *map_exist(ffan_map_t *arr, const char *key, size_t key_len) {
    map_item_t arr_item;
    arr_item.hash = map_key_hash(key, key_len);
    arr_item.key = key;
    arr_item.key_len = key_len;
    return map_exist_by_item(arr, &arr_item);
}

/**
 * 移除一个key
 * @param arr
 * @param key
 * @param key_len
 */
void map_unset(ffan_map_t *arr, const char *key, size_t key_len) {
    map_item_t arr_item;
    arr_item.hash = map_key_hash(key, key_len);
    arr_item.next = NULL;
    arr_item.key = key;
    arr_item.key_len = key_len;
    map_unset_by_item(arr, &arr_item);
}

/**
 * 获取数组的值
 * @param arr
 * @param key
 * @param key_len
 * @return
 */
void *map_get(ffan_map_t *arr, const char *key, size_t key_len) {
    map_item_t arr_item;
    arr_item.hash = map_key_hash(key, key_len);
    arr_item.key = key;
    arr_item.key_len = key_len;
    map_item_t *find_value = map_exist_by_item(arr, &arr_item);
    if (NULL == find_value) {
        return NULL;
    } else {
        return find_value->value;
    }
}

/**
 * 简单的遍历方法，无序的
 */
void map_walk(ffan_map_t *arr, map_walk_cb_pt cb) {
    int i;
    for (i = 0; i < arr->table_size; ++i) {
        if (NULL == arr->hash_table[i]) {
            continue;
        }
        map_item_t *head = arr->hash_table[i];
        map_item_t *current = NULL;
        while (head) {
            current = head;
            head = head->next;
            cb(current->key, current->key_len, current->value);
        }
    }
}