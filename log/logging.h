// Copyright (C) 2014 sails Authors.
// All rights reserved.
//
// Filename: logging.h
// Description: 可以直接通过new来得到一个logger,
//              但是推荐通过LoggerFactory达到单例目的
//              factory生成的日志默认是info级别,可以通过
//              修改log.confg:LogLevel=debug重新定义级别,合法的关键字:
//              debug, info ,warn, error,修改后10秒生效
//
// Author: sailsxu <sailsxu@gmail.com>
// Created: 2014-10-11 11:18:57



#ifndef SAILS_LOG_LOGGING_H_
#define SAILS_LOG_LOGGING_H_

#include <stdio.h>
#include <time.h>
#include <string>
#include <map>
#include <mutex>

namespace sails {
namespace log {

#define MAX_FILENAME_LEN 1000

class Logger {
 public:
  enum LogLevel{
    LOG_LEVEL_NONE =-1,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
  };

  enum SAVEMODE{
    SPLIT_NONE,
    SPLIT_MONTH,
    SPLIT_DAY,
    SPLIT_HOUR
  };

  explicit Logger(LogLevel level);
  Logger(LogLevel level, const char *filename);
  Logger(LogLevel level, const char *filename, SAVEMODE mode);

  void debug(const char* format, ...);
  void info(const char* format, ...);
  void warn(const char* format, ...);
  void error(const char* format, ...);

 private:
  void output(Logger::LogLevel level, const char* format, va_list ap);
  void set_msg_prefix(Logger::LogLevel level, char *msg);
  void set_filename_by_savemode(char* filename);
  void check_loginfo();
  Logger::LogLevel get_level_by_name(const char *name);
  void set_file_path();
  bool ensure_directory_exist();

  LogLevel level;
  char filename[MAX_FILENAME_LEN];
  char path[MAX_FILENAME_LEN];
  SAVEMODE save_mode;
  static char log_config_file[100];
  time_t update_loginfo_time;
};


class LoggerFactory {
 public:
  static Logger* getLog(std::string log_name);  // SPLIT_NONE
  static Logger* getLogD(std::string log_name);  // SPLIT_DAY
  static Logger* getLogH(std::string log_name);  // SPLIT_HOUR
  static Logger* getLogM(std::string log_name);  // SPLIT_MONTH
  
 private:
  static Logger* getLog(std::string log_name, Logger::SAVEMODE save_mode);
 private:
  static std::map<std::string, Logger*> log_map;
  static std::string path;
  static std::mutex logMutex;
};

}  // namespace log
}  // namespace sails

#endif  // SAILS_LOG_LOGGING_H_
