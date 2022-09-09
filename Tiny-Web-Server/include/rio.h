#pragma once
#include "golbal.h"

typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

void rio_readinitb(rio_t *rp, int fd);
int rio_readlineb(rio_t *rp, void *usrbuf, int maxlen);
int rio_read(rio_t *rp, char *userbuf, int n);
int rio_writen(int fd, void *usrbuf, int n);