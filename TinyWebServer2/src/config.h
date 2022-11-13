#pragma once

#include <toml++/toml.h>
#include <boost/filesystem.hpp>
#include <boost/dll.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <map>

class config {
// 配置文件还是要写成单列模式，以便于在代码的各个地方都能获取到
// 单列模式，意味着构造的时候，无法传入参数，所以我们需要加一个parse函数
// 参数有很多，如果每个都弄个get函数，比较罗嗦，方便的方式是i查表，比如数组，或者map。
// 使用数组，意味着我们还需要进行宏定义和定义一个封装类，比较严谨，但有点麻烦
// 这里使用map，简单，但不严谨
private:
  std::map<std::string, std::string> m_cfg_str;
  std::map<std::string, int> m_cfg_int;
private:
  config() = default;
public:
  config(const config&) = delete;
  config& operator=(const config&) = delete;
  static config& instance();
  void parse_toml(boost::filesystem::path &cfg_path);
  void get(const std::string name, std::string &val);
  void get(const std::string name, int &val);
};