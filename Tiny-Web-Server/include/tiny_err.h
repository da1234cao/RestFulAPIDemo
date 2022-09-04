#pragma once

#include "golbal.h"
#include <stdio.h>

#define LOGGER user_err_doit
#define log_init err_init
#define log_exit err_exit

void err_init(int err_to_file);
// void err_init(int err_to_file = 0); // C语言没有默认参数

void err_exit();

void user_err_doit(const char *fmt, ...);

void sys_err_doit(const char *fmt, ...);

