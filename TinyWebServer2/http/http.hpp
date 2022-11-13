#pragma once

#include "common/util.hpp"
#include "common/log.hpp"
#include "request.hpp"
#include "message_board.hpp"
#include <boost/beast/http.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <unistd.h>
#include <string.h>
#include <string>

#define TMP_BUFFER_SIZE 2048

namespace tiny {
enum http_status {
  READABLE,
  WRITEBALE,
  UNKNOW
};

class http {
private:
  char tmp_buf[TMP_BUFFER_SIZE];
private:
  http_status m_status = UNKNOW;
  std::string m_recv_buf;
  std::string m_send_buf;
  tiny::request m_request;
  int m_epollfd;
  int m_sockfd;
public:
  http(int epollfd, int sockfd): m_epollfd(epollfd), m_sockfd(sockfd) {}
  void process() {
    if(m_status == UNKNOW || m_status == READABLE) {
      // 读处理
      int bytes_read = read(m_sockfd, tmp_buf, TMP_BUFFER_SIZE);
      if(bytes_read < 0) {
        Log::LOG_ERROR("read fail.the erron is:{}", errno);
        return;
      } else if(bytes_read == 0) {
        Log::LOG_DEBUG("sockfd {} read process end.", m_sockfd);
        utils::epoll_help::instance().modfd(m_epollfd, m_sockfd, EPOLLRDHUP);
        return;
      }
      Log::LOG_TRACE("socket {} receive: {}", m_sockfd, std::string(tmp_buf)); 
      m_recv_buf.append(std::string(tmp_buf, bytes_read));

      // 当获取到完整的http请求后，将http状态修改为可写 -- 准备response
      // 同时设置相关联的fd可写
      m_request.data(m_recv_buf.c_str(), m_recv_buf.size());
      m_request.execute();
      if(m_request.is_valid()) {
        std::map<std::string, std::string> request_heads;
        m_request.get_headers(request_heads);
        std::string url = "http://" + request_heads["Host"] + m_request.get_url(); // 不提供https接口
        Log::LOG_DEBUG("url is: {}", url);
        if(url.find("message_board") != std::string::npos) {
          message_board::execute(m_request.get_method(), url, m_send_buf);
        }
        m_status = WRITEBALE;
        utils::epoll_help::instance().modfd(m_epollfd, m_sockfd, EPOLLOUT);
      }
    }else if(m_status == WRITEBALE) {
      // 写处理
      int sended = 0;
      while(sended < m_send_buf.size()) {
        int n = write(m_sockfd, m_send_buf.c_str() + sended, m_send_buf.size() - sended);
        Log::LOG_TRACE("socket {} send: {}", m_sockfd, m_send_buf.substr(sended, n));
        sended += n;
      }

      // 写完毕，进行状态转换
      m_recv_buf.clear();
      m_status = READABLE;
      if(m_status == READABLE) {
        utils::epoll_help::instance().modfd(m_epollfd, m_sockfd, EPOLLIN);
      }
    }
  }
};
} // tiny namespace