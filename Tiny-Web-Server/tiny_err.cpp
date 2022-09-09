#include "tiny_err.h"
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 该文件参考：https://github.com/da1234cao/APUE/blob/master/lib/error.c

FILE* log_fd;
int ERR_TO_FILE = 0;

// 只要声明这个静态函数即可。其他已经在头文件中声明，不会存在调用的时候，没有声明问题
static void err_doit(int errorflag,int error, const char *fmt, va_list ap);

void err_init(int err_to_file) {
    if(err_to_file) {
        ERR_TO_FILE = 1;
        log_fd = fopen(LOG_FILE, "a+");
    }
}

void err_exit(void) {
    if(log_fd) {
        fflush(log_fd);
        fclose(log_fd);
    }
}

void user_err_doit(const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
    err_doit(0, 0, fmt, ap);
}

void sys_err_doit(const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
    err_doit(1, errno, fmt, ap);
}

static void err_doit(int errorflag,int error, const char *fmt, va_list ap){
    
    char buf[ERR_BUF];
    vsnprintf(buf,ERR_BUF-1,fmt,ap);
    if(errorflag){
        snprintf(buf+strlen(buf),ERR_BUF-strlen(buf)-1,":%s",strerror(error));
    }
    strcat(buf,"\n");
    if(ERR_TO_FILE) {
        fputs(buf,log_fd);
    } else {
        fflush(stdout);
        fputs(buf,stderr);
    }
}