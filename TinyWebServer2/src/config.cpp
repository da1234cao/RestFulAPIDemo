#include "config.h"

config& config::instance() {
  static config inst;
  return inst;
}

void config::parse_toml(boost::filesystem::path &cfg_path) {
  toml::table tlbs = toml::parse_file(cfg_path.string());
  // 日志文件
  m_cfg_str["log_file_path"] = tlbs["log"]["path"].value<std::string>().value();
	m_cfg_str["log_level"] = tlbs["log"]["level"].value<std::string>().value();
	m_cfg_int["max_log_file_size"] = tlbs["log"]["max_file_size"].value<int>().value();
	m_cfg_int["max_log_files"] = tlbs["log"]["max_files"].value<int>().value();

  // 服务器配置
  m_cfg_int["port"] = tlbs["server"]["port"].value<int>().value();
  m_cfg_int["max_thread_num"] = tlbs["server"]["max_thread_num"].value<int>().value();

  // sql数据库信息
  m_cfg_str["sql_name"] = tlbs["sql"]["name"].value<std::string>().value();
  m_cfg_str["sql_password"] = tlbs["sql"]["password"].value<std::string>().value();
  m_cfg_str["sql_host"] = tlbs["sql"]["host"].value<std::string>().value();
  m_cfg_str["sql_schema"] = tlbs["sql"]["schema"].value<std::string>().value();
  m_cfg_str["sql_table"] = tlbs["sql"]["table"].value<std::string>().value();
  m_cfg_int["sql_pool_size"] = tlbs["sql"]["pool_size"].value<int>().value();
}

void config::get(const std::string name , std::string &val) {
  val = m_cfg_str[name];
}

void config::get(const std::string name, int &val) {
  val = m_cfg_int[name];
}