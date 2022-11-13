#pragma once

#include "config.h"
#include "common/log.hpp"
#include <mysql-cppconn-8/mysqlx/xdevapi.h>
#include <memory>

class sql {
private:
  sql() = default;
  std::shared_ptr<mysqlx::Client> m_cli_ptr;
public:
  sql(const sql&) = delete;
  sql& operator=(const sql&) = delete;
  static sql& instance() {
    static sql instance;
    return instance;
  } 

  void init() {
    std::string sql_name, sql_password, sql_host;
    int sql_pool_size;
    config::instance().get("sql_name", sql_name);
    config::instance().get("sql_password", sql_password);
    config::instance().get("sql_host", sql_host);
    config::instance().get("sql_pool_size", sql_pool_size);

    std::string sql_url = sql_name + ":" + sql_password + '@' + sql_host;
    m_cli_ptr = std::make_shared<mysqlx::Client>(sql_url, mysqlx::ClientOption::POOL_MAX_SIZE, sql_pool_size);
    Log::LOG_DEBUG("sql pool has inited.");
  }

  mysqlx::Session getSession() {
    return m_cli_ptr->getSession();
  }

  ~sql() {
    m_cli_ptr->close();
  }
};