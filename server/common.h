//
// Created by bluebird on 2016-9-14.
//

#ifndef im_COMMON_H
#define im_COMMON_H

#include "yile.h"
#include "yile_buf.h"
#include "yile_net.h"
#include "yile_ini.h"
#include "yile_log.h"
#include "yile_process.h"
#include "yile_protocol.h"
#include "log.h"
#include <time.h>

//通用的失败标志
#define RESULT_FAIL 1

//通用的成功标志
#define RESULT_SUCCESS 0

//分配内存
#define im_malloc( p, size ) p = malloc(size); if (NULL == p){log_emergency("Out of memory at: %s %d", __FILE__, __LINE__);exit(1);}
//复制一个字符串
#define im_strdup(p, str) p = strdup(str); if (NULL == p){log_emergency("Out of memory at: %s %d", __FILE__, __LINE__);exit(1);}

#endif //im_COMMON_H
