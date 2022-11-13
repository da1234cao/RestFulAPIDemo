#include "common/exception.hpp"
#include "common/log.hpp"
#include "common/sql.hpp"
#include "config.h"
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
    config::instance().parse_toml(cfg_path);
    
    // 初始化日志配置
    std::string log_file_path, log_level;
    int max_log_files, max_log_file_size;
    config::instance().get("log_file_path", log_file_path);
    config::instance().get("log_level", log_level);
    config::instance().get("max_log_files", max_log_files);
    config::instance().get("max_log_file_size", max_log_file_size);
		Log::SPDLOG::getInstance().init(log_file_path, "default_logger", log_level, 
                                    max_log_file_size * 1024 * 1024, max_log_files, false);
		Log::LOG_DEBUG("log has inited.");

    // 初始化数据库
    sql::instance().init();

    // 启动server
    int port, max_thread_num;
    config::instance().get("port", port);
    config::instance().get("max_thread_num", max_thread_num);
    Log::LOG_DEBUG("begain start server.port is {}", port);
    server serv(port, max_thread_num);
    serv.start();
  } 
  catch(base_exception& e) {
    std::cerr << boost::diagnostic_information(e) << std::endl;
  }

}