#pragma once

#include "common/sql.hpp"
#include "common/exception.hpp"
#include "http/request.hpp"
#include <boost/beast/http.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/url.hpp>
#include <boost/json.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <algorithm>
#include <utility>
#include <sstream>

class message_board {
private:
  static std::string vec_pair_to_string(std::vector<std::pair<std::string, std::string>> &info);
public:
  message_board(const message_board&) = delete;
  message_board& operator=(const message_board&) = delete;
  message_board() = default;
  static void execute(tiny::request &request, std::string &resp_str);
};

void message_board::execute(tiny::request &request, std::string &resp_str) {
  std::map<std::string, std::string> request_heads;
  request.get_headers(request_heads);
  std::string url = "http://" + request_heads["Host"] + request.get_url(); // 不提供https接口
  Log::LOG_DEBUG("url is: {}", url);
  std::string method = request.get_method();
  std::transform(method.begin(), method.end(), method.begin(), ::toupper);
  std::string path = boost::urls::parse_uri(url).value().path();

  // 这个函数太长了.最好分成不同的方法
  if(method == "GET" && path == "/message_board/all") {
    std::vector<std::pair<std::string, std::string>> resp_body_vec;

    std::string sql_schema, sql_table;
    config::instance().get("sql_schema", sql_schema);
    config::instance().get("sql_table", sql_table);
    mysqlx::Session sess = sql::instance().getSession();
    mysqlx::Schema db= sess.getSchema("message_board");
    mysqlx::Table tb = db.getTable("message");
    mysqlx::RowResult result = tb.select("*").execute();
    std::list<mysqlx::Row> rows = result.fetchAll();
    for(auto row : rows) {
      resp_body_vec.emplace_back(std::make_pair(row[0], row[1]));
    }

    namespace http = boost::beast::http;
    http::response<http::string_body> resp;
    resp.set(http::field::server, "tiny-server"); 
    resp.set(http::field::access_control_allow_origin, "*"); 
    resp.set(http::field::content_type, "application/json;charset=utf8");
    resp.body() = vec_pair_to_string(resp_body_vec); 
    resp.prepare_payload(); 
    resp.result(http::status::ok);
    resp_str = boost::lexical_cast<std::string>(resp.base()) + std::string(resp.body().data());
    return;
  } 
  else if(method == "POST" && path == "/message_board") {
    std::string body_str = request.get_body();
    Log::LOG_DEBUG("{} body is {}", path, body_str);

    namespace http = boost::beast::http;
    http::response<http::string_body> resp;
    resp.set(http::field::server, "tiny-server"); 
    resp.set(http::field::access_control_allow_origin, "*"); 
    resp.set(http::field::content_type, "application/json;charset=utf8");
    try {
      boost::system::error_code ec; // error code还咋会用
      boost::json::value body_value = boost::json::parse(body_str, ec);
      boost::json::object body_obj = body_value.as_object();

      std::string name = boost::json::string_view(body_obj["name"].as_string());
      std::string message = boost::json::string_view(body_obj["message"].as_string());
      
      std::string sql_schema, sql_table;
      config::instance().get("sql_schema", sql_schema);
      config::instance().get("sql_table", sql_table);
      mysqlx::Session sess = sql::instance().getSession();
      mysqlx::Schema db= sess.getSchema("message_board");
      mysqlx::Table tb = db.getTable("message");
      tb.insert("username", "message").values(name, message).execute();
      resp.result(http::status::ok);
    }
    catch (const std::exception) {
      // 异常分类，我有时间整理下，这里统一使用它接受
      resp.result(http::status::bad_request);
      Log::LOG_INFO("bad request: {}", request.to_string());
    }
    
    resp.prepare_payload(); 
    resp_str = boost::lexical_cast<std::string>(resp.base());
    return;
  } else {
    // 没有匹配上
    Log::LOG_INFO("bad request. not match: {}", request.to_string());
    namespace http = boost::beast::http;
    http::response<http::string_body> resp;
    resp.set(http::field::server, "tiny-server"); 
    resp.set(http::field::access_control_allow_origin, "*"); 
    resp.set(http::field::content_type, "application/json;charset=utf8");
    resp.result(http::status::not_found);
    resp.prepare_payload(); 
    resp_str = boost::lexical_cast<std::string>(resp.base()) + std::string(resp.body().data());
    return;
  }
}

std::string message_board::vec_pair_to_string(std::vector<std::pair<std::string, std::string>> &info) {
    boost::json::array info_array;
    for(auto &in : info) {
      boost::json::object obj;
      obj["name"] = in.first;
      obj["message"] = in.second;
      info_array.emplace_back(obj);
    }
    boost::json::object info_json;
    info_json.emplace("info", info_array);
    return boost::json::serialize(info_json);
}