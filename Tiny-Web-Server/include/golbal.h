#pragma once

// 没有使用配置文件，这里使用一个全局的头文件
// 该文件仅允许定义宏；该文件禁止定义全局变量，避免multiple definition

#define ERR_BUF 1024

#define RECV_BUF 1024

#define LOG_FILE "log.txt"

#define PORT 9999

#define LISTENQ 20 // 不知道设置多大，随意设置一个

#define MAXCLIENT 15 // 同时连接的最大客户端数量；要小于上面的LISTENQ

#define RIO_BUFSIZE 8192

#define USER_BUFSIZE 8129