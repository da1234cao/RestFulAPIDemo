#include "exception.hpp"
#include "config.h"
#include "log.hpp"
#include "server.hpp"
#include <iostream>

int main(int argc, char** argv)
{
  try {
    // boost::filesystem::path cur_path = boost::filesystem::current_path(); // 这是cwd,不一定是程序所在目录
    // https://stackoverflow.com/questions/5694190/boost-filesystem-how-to-find-out-in-which-directory-your-executable-is
    boost::filesystem::path cur_path = boost::dll::program_location().parent_path();
    boost::filesystem::path cfg_path = cur_path / "config.toml";

    // 解析配置文件
    config cfg;
    cfg.parse_toml(cfg_path);

    // 初始化日志配置
		Log::SPDLOG::getInstance().init(cfg.log_file_path, "default_logger", cfg.log_level, 
                                    cfg.max_log_file_size * 1024 * 1024, cfg.max_log_files, false);
		Log::LOG_DEBUG("log has inited.");

    Log::LOG_DEBUG("begain start server.port is {}", cfg.port);
    server serv(cfg.port, cfg.max_thread_num);
    serv.start();
  } 
  catch(base_exception& e) {
    std::cerr << boost::diagnostic_information(e) << std::endl;
  }

}