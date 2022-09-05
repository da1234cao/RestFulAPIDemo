#include "tiny_err.h"
#include <sys/socket.h>
#include <bits/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>

// 没有std,写且东西来有点束手束脚
// 插入一个fd,各种属性可以自动维护。有序set是挺好的结构
struct conn_fd_st {
    int maxi; // 距离数组开头最远的描述符下标(只增大不减小.不想单独建立一个文件写各种方法^__)
    int nfds; // fd中最大值加1(只增大不减小.不想单独建立一个文件写各种方法^__)
    int conn_fd[MAXCLIENT]; // FD_ISSET将描述符集中的对应位清零，没有重新置1。需要维护一个数组来记录有哪些套接字
};

int main(int argc, char *argv[])
{
    log_init(1);
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        sys_err_doit("create listen fd failed.");
        return -1;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if(bind(listenfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        sys_err_doit("bind a name to listen socket failed.");
        return -1;
    }

    if(listen(listenfd, LISTENQ) < 0) {
        sys_err_doit("listen funciton failed.");
        return -1;
    }

    struct conn_fd_st conn_fds;
    for(int i=0; i<MAXCLIENT; i++) {
        conn_fds.conn_fd[i] = -1;
    }
    conn_fds.conn_fd[0] = listenfd;
    conn_fds.maxi = 0;
    conn_fds.nfds = listenfd + 1;

    fd_set readfds;
    FD_ZERO(&readfds);
    // 有没有必要在select中，知道一个套接字何时能写？
    // 不知道什么时候可读，因为不知道对面什么时候发送；但想写的话，不是应该立马能写？
    // fd_set writefds; 
    // FD_ZERO(&writefds);

    while(1) {
        // 每次循环开头设置fd_set
        FD_SET(listenfd, &readfds);
        for(int i=1; i<=conn_fds.maxi; i++) {
            int socket_fd = conn_fds.conn_fd[i];
            if(socket_fd == -1) 
                continue;
            FD_SET(socket_fd, &readfds);
        }

        int nready = select(conn_fds.nfds, &readfds, NULL, NULL, NULL);

        // 准备建立新的连接
        if(FD_ISSET(listenfd, &readfds)) {
            int i;
            for(i=1; i<MAXCLIENT; i++) {
                if(conn_fds.conn_fd[i] == -1) {
                    int connfd = accept(listenfd, NULL, NULL); // 有空间后，再去获取新资源
                    conn_fds.conn_fd[i] = connfd;
                    if(i > conn_fds.maxi) {
                        conn_fds.maxi = i;
                    }
                    if(connfd >= conn_fds.nfds) {
                        conn_fds.nfds = connfd + 1;
                    }
                    nready--;
                    break;
                }
            }
            if(i == MAXCLIENT) {
                user_err_doit("too many client: %d\n", i);
            }
        }

        for(int i=0; i<= conn_fds.maxi; i++) {
            int socket_fd = conn_fds.conn_fd[i];
            if(socket_fd == -1)
                continue;
            char buff[RECV_BUF];
            if(FD_ISSET(socket_fd, &readfds)){
                int n = read(socket_fd, buff, RECV_BUF);
                if(n == 0) { // 客户端关闭连接
                    close(socket_fd);
                    FD_CLR(socket_fd, &readfds);
                    conn_fds.conn_fd[i] = -1;
                } else {
                    write(socket_fd, buff, n); // 回射服务器，测试连接情况
                }
                nready--;
                if(nready <= 0)
                    break;
            }
        }
    }

    log_exit();
    return 0;
}