#!/bin/env bash

# 环境变量
ENV=$1
ENV=$(echo $ENV | tr '[A-Z]' '[a-z]') #统一转换成小写

mkdir output
rsync -aIr --exclude="output" `pwd`"/" ./output/

echo $ENV

# 复制配置文件
if [ "$ENV" == "sit" ] ;then #复制sit环境
    CMD="mv ./output/config/config_sit.ini ./output/config/config.ini"
elif [ "$ENV" == "uat" ] ;then #复制uat环境
    CMD="mv ./output/config/config_uat.ini ./output/config/config.ini"
else #其余的都是正式环境
    CMD="mv ./output/config/config_prod.ini ./output/config/config.ini"
fi

echo $CMD

$CMD || exit 1

exit 0