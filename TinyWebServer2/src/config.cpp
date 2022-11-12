#include "config.h"

void config::parse_toml(boost::filesystem::path &cfg_path) {
  toml::table tlbs = toml::parse_file(cfg_path.string());
  log_file_path = tlbs["log"]["path"].value<std::string>().value();
	log_level = tlbs["log"]["level"].value<std::string>().value();
	max_log_file_size = tlbs["log"]["max_file_size"].value<int>().value();
	max_log_files = tlbs["log"]["max_files"].value<int>().value();
  port = tlbs["server"]["port"].value<int>().value();
  max_thread_num = tlbs["server"]["max_thread_num"].value<int>().value();
}
