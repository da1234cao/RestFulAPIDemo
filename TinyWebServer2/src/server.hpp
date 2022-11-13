#pragma once


#include "http/http.hpp"
#include "thread_pool.hpp"
#include "common/util.hpp"
#include "common/exception.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>
#include <map>
#include <memory>

#define MAX_EVENT_NUMBER 10000

struct server_exception : base_exception {};

class server {
private:
  int m_port;
  int m_listenfd;
  int m_epollfd;
  bool m_stop = false;
  thread_pool<tiny::http> pool; // 处理resolve对象的线程池
  std::map<int, std::shared_ptr<tiny::http>> connects; // 存储从客户端发送来的内容，存储回复给客户端的内容

private:
  void event_listen();
  void event_loop();
public:
  server(int port = 9999, int max_thread_num = 3);
  ~server();
  void start();
};

server::server(int port, int max_thread_num): m_port(port), pool(max_thread_num) {}

server::~server() {
  close(m_epollfd);
}

void server::start() {
  event_listen(); 
  event_loop();
}

void server::event_listen() {
  // 创建监听描述符并加入epoll set
  m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
  if(m_listenfd <= 0) {
    Log::LOG_ERROR("create listen socket fail.");
    BOOST_THROW_EXCEPTION(server_exception() << err_str("create listen socket fail."));
  }

  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(m_port);

  int ret = bind(m_listenfd, (struct sockaddr *)&address, sizeof(address));
  if(ret < 0) {
    Log::LOG_ERROR("failed to bind socket.");
    BOOST_THROW_EXCEPTION(server_exception() << err_str("failed to bind socket."));
  }
  ret = listen(m_listenfd, 5);
  if(ret < 0) {
    Log::LOG_ERROR("failed to bind socket.");
    BOOST_THROW_EXCEPTION(server_exception() << err_str("failed to listen socket."));
  }

  m_epollfd = epoll_create(5);
  if(m_epollfd == -1) {
    Log::LOG_ERROR("failed to call epoll_create function.");
    BOOST_THROW_EXCEPTION(server_exception() << err_str("failed to call epoll_create function."));
  }
  utils::epoll_help::instance().addfd(m_epollfd, m_listenfd, false);
}

void server::event_loop()
{
  epoll_event events[MAX_EVENT_NUMBER];
  while(!m_stop) {
    int n = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
    if(n < 0 && errno == EINTR) {
      Log::LOG_DEBUG("epoll EINTR");
      continue;
    } else if(n < 0 && errno != EINTR) {
      Log::LOG_ERROR("epoll wait fail: {}",  strerror(errno));
      BOOST_THROW_EXCEPTION(server_exception() << err_str("epoll wait fail."));
    }

    for(int i=0; i<n; i++) {
      int sockfd = events[i].data.fd;
      if(sockfd == m_listenfd) { // 建立新的连接
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        int connfd = accept(m_listenfd, (struct sockaddr *)&client_address, &client_addr_len);
        if(connfd < 0) {
          Log::LOG_ERROR("accept fail.");
          continue;
        }
        utils::epoll_help::instance().addfd(m_epollfd, connfd, true);
        connects[connfd] = std::shared_ptr<tiny::http>(new tiny::http(m_epollfd, connfd));
      } else if(events[i].events & EPOLLIN) { // 读-丢给线程池处理
        pool.append(connects[sockfd]);
      } else if(events[i].events & EPOLLOUT) { // 写-丢给线程池处理
        pool.append(connects[sockfd]);
      } else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) { // 客户端关闭
        utils::epoll_help::instance().removefd(m_epollfd, events[i].data.fd);
        // connects.erase(events[i].data.fd); // 这样擦出或许不好，要改变树结构
        // 所以不用删除，fd已经关闭，不会使用connects[fd];当新的相同的fd连接时，自动覆盖
      }
    }
  }
}