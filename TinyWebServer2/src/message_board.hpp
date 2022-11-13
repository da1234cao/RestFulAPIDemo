#pragma once

#include "common/sql.hpp"
#include <boost/beast/http.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/url.hpp>
#include <boost/json.hpp>
#include <string>
#include <algorithm>
#include <utility>

class message_board {
private:
  static std::string vec_pair_to_string(std::vector<std::pair<std::string, std::string>> &info);
public:
  message_board(const message_board&) = delete;
  message_board& operator=(const message_board&) = delete;
  message_board() = default;
  static void execute(std::string method, const std::string &url, std::string &resp_str);
};

void message_board::execute(std::string method, const std::string &url, std::string &resp_str) {
  std::transform(method.begin(), method.end(), method.begin(), ::toupper);
  if(method == "GET") {
    std::string path = boost::urls::parse_uri(url).value().path();
    if(path == "/message_board/all") {
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
    }
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