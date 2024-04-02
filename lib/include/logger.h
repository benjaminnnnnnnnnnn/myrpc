#pragma once
#include "lockqueue.h"
#include <string>

//define LOG_INFO
#define LOG_INFO(logmsgformat, ...)\
    do\
    {\
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(INFO);\
        char c[1024] = {0};\
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__);\
        logger.Log(c);\
    }while(0)\

//define LOG_ERROR
#define LOG_ERROR(logmsgformat, ...)\
    do\
    {\
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(ERROR);\
        char c[1024] = {0};\
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__);\
        logger.Log(c);\
    }while(0)\

enum LogLevel{
    INFO,   //normal info
    ERROR,  //error info
};

//myrpc log system
class Logger{
public:
    //One instance
    static Logger& GetInstance();
    //set log level
    void SetLogLevel(LogLevel level);
    //record log
    void Log(std::string msg);
private:
    int m_loglevel; //record log level
    LockQueue<std::string> m_lckQue; //log buffer queue

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};