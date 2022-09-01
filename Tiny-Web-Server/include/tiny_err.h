#pragma once

#include "golbal.h"
#include <stdio.h>

#define LOGGER user_err_doit

void err_init(int err_to_file);
// void err_init(int err_to_file = 0); // C语言没有默认参数

void err_exit();

void user_err_doit(const char *fmt, ...);

void sys_err_doit(int error, const char *fmt, ...);

