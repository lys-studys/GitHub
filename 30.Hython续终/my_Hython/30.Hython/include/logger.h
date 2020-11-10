/*************************************************************************
	> File Name: logger.h
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2020年04月25日 星期六 15时09分51秒
 ************************************************************************/

#ifndef _LOGGER_H
#define _LOGGER_H
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <map>
#include <string>
#define LOG_TRACE(level) haizei::haizei_log(__FILE__, __func__, __LINE__, level)
#define LOG_INFO LOG_TRACE(haizei::LogLevel::INFO)
#define LOG_DEBUG LOG_TRACE(haizei::LogLevel::DEBUGD)
#define LOG_WARN LOG_TRACE(haizei::LogLevel::WARN)
#define LOG_ERROR LOG_TRACE(haizei::LogLevel::ERROR)
#define LOG_SET(levels) haizei::Logger::set_level(levels)

namespace haizei {

class LogLevel {
public :
    static const int INFO;
    static const int DEBUGD;
    static const int WARN;
    static const int ERROR;
};
const int LogLevel::INFO = 1;
const int LogLevel::DEBUGD = 2;
const int LogLevel::WARN = 4;
const int LogLevel::ERROR = 8;

std::map<int, std::string> LogLevelStr = {
    {LogLevel::INFO, "INFO"},       
    {LogLevel::DEBUGD, "DEBUGD"},       
    {LogLevel::WARN, "WARN"},       
    {LogLevel::ERROR, "ERROR"},       
};

class Logger {
    class LoggerStream : public std::ostringstream {
    public :
        LoggerStream(
            const char *file_name, 
            const char *func_name,
            int line_no,
            Logger &raw_obj,
            int l
        ) :
        raw_obj(raw_obj),
        line_no(line_no),
        flag(1) {
            std::ostringstream &now = *this;
            now << "[" << file_name << " project]";
            now << " [" << func_name << "]";
            now << " [" << LogLevelStr[l] << "] ";
            flag = Logger::logger_level_conf & l;
        }
        LoggerStream(const LoggerStream &ls) : raw_obj(ls.raw_obj) {}
        ~LoggerStream() {
            if (flag) output();
        }
    private:
        void output() {
            std::unique_lock<std::mutex> lock(raw_obj.m_mutex);
            std::ostringstream &now = *this;
            now << " (" << line_no << ")";
            std::cout << this->str() << std::endl;
            std::cout.flush();
            return ;
        }
        Logger &raw_obj;
        int line_no, flag;
    };
public :
    LoggerStream operator()(const char *file_name, const char *func_name, int line_no, int l) {
        return LoggerStream(file_name, func_name, line_no, *this, l);
    }
    static void set_level(int levels) {
        Logger::logger_level_conf = levels;
        return ;
    }
private:
    std::mutex m_mutex;
    static int logger_level_conf;
};

int Logger::logger_level_conf = 15;

Logger haizei_log;

}


#endif
