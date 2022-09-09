#include "rio.h"
#include <unistd.h>
#include <cstring>



/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}

// 有可能是，也有可能不是一行。根据最后一个字符判断（尽力去读取一个字符串）
int rio_readlineb(rio_t *rp, void *usrbuf, int maxlen) 
{
    char c;
    char *buf = (char*)usrbuf;
    int i;
    for(i=1; i<maxlen; i++) {
        if(rio_read(rp, &c, 1) == 1) {
            *buf++ = c;
            if(c == '\n') {
                i++;
                break;
            }
        } else {
            return 0;
        }
    }
    *buf = 0;
    return i-1;
}

int rio_read(rio_t *rp, char *userbuf, int n)
{
    if(rp->rio_cnt <= 0) { // 重新填充
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if(rp->rio_cnt == 0) {
            return 0;
        }
        rp->rio_bufptr = rp->rio_buf;
    }
    
    int cnt = (n < rp->rio_cnt) ? n : rp->rio_cnt;
    memcpy(userbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

/*
 * rio_writen - robustly write n bytes (unbuffered)
 */
int rio_writen(int fd, void *usrbuf, int n) 
{
    int nleft = n;
    int nwritten;
    char *bufp = (char*)usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}