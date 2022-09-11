#include "http.h"
#include "golbal.h"
#include "rio.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
     char buf[MAXLINE], body[USER_BUFSIZE];

     /* Build the HTTP response body */
     sprintf(body, "<html><title>Tiny Error</title>");
     sprintf(body, "%s<body> bgcolor=""ffffff"">\r\n", body);
     sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
     sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
     sprintf(body, "%s<hr><em>The Tiny Web server </em>\r\n", body);

     /* Print the HTTP response */
     sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
     rio_writen(fd, buf, strlen(buf));
     sprintf(buf, "Content-type: text/html\r\n");
     rio_writen(fd, buf, strlen(buf));
     sprintf(buf, "Content-length: %d\r\n\r\n",(int)strlen(body));
     rio_writen(fd, buf, strlen(buf));
     rio_writen(fd, body, strlen(body));
}

void serve_static(int fd, char *filename, int filesize)
{
     int srcfd;
     char *srcp, filetype[MAXLINE], buf[USER_BUFSIZE];

     /* Send reponse headers to client */
     get_filetype(filename, filetype);
     sprintf(buf, "HTTP/1.0 200 OK\r\n");
     sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
     sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
     sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
     rio_writen(fd, buf, strlen(buf));

     /* Send reqponse body to client */
     srcfd = open(filename, O_RDONLY, 0);
     srcp = (char*)mmap(0, filesize,  PROT_READ, MAP_PRIVATE, srcfd, 0);
     close(srcfd);
     rio_writen(fd, srcp, filesize);
     munmap(srcp, filesize);
}

void get_filetype(char *filename, char *filetype)
{
     if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
     else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
     else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
     else
        strcpy(filetype, "text/plain");
}