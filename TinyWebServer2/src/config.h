#pragma once

#include <toml++/toml.h>
#include <boost/filesystem.hpp>
#include <boost/dll.hpp>
#include <string>

typedef struct config {
  std::string log_file_path;
  std::string log_level;
  int max_log_file_size;
  int max_log_files;
  int port;
  int max_thread_num;

  config() = default;
  config(const config&) = delete;
  config& operator=(const config&) = delete;
  // const config& instance();
  void parse_toml(boost::filesystem::path &cfg_path);
}config;