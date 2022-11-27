#ifndef __LOGGER_H__
#define __LOGGER_H__

#include<stdint.h>
#include<string>
#include<sstream>
#include<memory>
#include<iostream>
#include<vector>
#include"loglevel.h"

namespace server{

class Logger;

class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger,const char* filename,uint32_t line, \
    uint32_t elapse,uint32_t threadId,uint32_t fiberId, \
    uint64_t time,const std::string& threadName,LogLevel::Level level);
    ~LogEvent(){}
    const char* getFileName() const {return m_fileName;}
    uint32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    uint32_t getFiberId() const {return m_fiberId;}
    uint64_t getTime() const {return m_time;}
    const std::string& getThreadName() const {return m_threadName;}
    std::string getContent() const {return m_ss.str();}
    LogLevel::Level getLevel() const {return m_level;}
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
    std::stringstream& getSS() {return m_ss;}
    void format(const char* fmt, ...);
    void format(const char* fmt,va_list al);

private:
    const char* m_fileName = nullptr;
    uint32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time = 0;
    std::string m_threadName;
    std::stringstream m_ss;
    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    /**
     *  @param[in] pattern 格式模板
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %F 协程id
     *  %N 线程名称
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    LogFormatter(const std::string& pattern);
    ~LogFormatter();
    void init();
    std::string format(std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level);
    std::string format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level);
    const std::string getPattern() const {return m_pattern;};
    bool isError() const {return m_error;}
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) = 0;
    };
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error = false;
};

class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;

    const std::string& getName() const {return m_name;}
private:
    std::string m_name;
};

}

#endif