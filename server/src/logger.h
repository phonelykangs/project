#ifndef __LOGGER_H__
#define __LOGGER_H__

#include<stdint.h>
#include<string>
#include<sstream>
#include<memory>
#include<iostream>
#include<vector>
#include<fstream>
#include<list>
#include<map>
#include"mutex.h"
#include"singleton.h"
#include"utils.h"

#define LOG(logger,level) \
    if(logger->getLevel() <= level) \
        server::LogEventWrap(std::make_shared<server::LogEvent>(logger,__FILE__,__LINE__, \
        0,server::GetThreadId(),0,time(nullptr),"test",level)).getSS();

#define LOG_DEBUG(logger) LOG(logger,server::LogLevel::Level::DEBUG)
#define LOG_INFO(logger) LOG(logger,server::LogLevel::Level::INFO)
#define LOG_WARN(logger) LOG(logger,server::LogLevel::Level::WARN)
#define LOG_ERROR(logger) LOG(logger,server::LogLevel::Level::ERROR)
#define LOG_FATAL(logger) LOG(logger,server::LogLevel::Level::FATAL)

#define GET_LOG_ROOT() LogMgr::GetInstance()->getRoot()
#define GET_LOG_NAME(name) LogMgr::GetInstance()->getLogger(name)

namespace server{

class Logger;

class LogLevel{
public:

    enum Level{
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string& str);
};

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
    std::shared_ptr<Logger> m_logger;
    const char* m_fileName = nullptr;
    uint32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time = 0;
    std::string m_threadName;
    std::stringstream m_ss;
    LogLevel::Level m_level;
};

class LogEventWrap{
public:
    LogEventWrap(LogEvent::ptr event):m_event(event){}
    ~LogEventWrap();
    LogEvent::ptr getEvent() const {return m_event;}
    std::stringstream& getSS() {return m_event->getSS();}
private:
    LogEvent::ptr m_event;
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
    ~LogFormatter(){};
    void init();
    std::string format(std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level);
    std::ostream& format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level);
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

class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    typedef Spinlock MutexType;
    virtual ~LogAppender(){}
    LogFormatter::ptr getFormater();
    LogLevel::Level getLevel();
    void setLevel(LogLevel::Level level);
    void setFormater(LogFormatter::ptr formater);
    virtual std::string toYamlString() = 0;
    virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
    bool hasFormater();
protected:
    //MutexType m_mutex;
    LogFormatter::ptr m_formater;
    bool m_hasFormater = false;
    LogLevel::Level m_level = LogLevel::Level::DEBUG;
};

class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef Spinlock MutexType;
    Logger(const std::string& name = "root");
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    const std::string& getName() const {return m_name;}
    void setFormater(const std::string& pattern);
    void setFormater(const LogFormatter::ptr formater);
    void addAppender(const LogAppender::ptr appender);
    void delAppender(const LogAppender::ptr appender);
    void log(LogLevel::Level level,LogEvent::ptr event);
    void clearAppenders();
    LogLevel::Level getLevel();
    void setLevel(LogLevel::Level level);
    std::string toYamlString();
    LogFormatter::ptr getFormater();
    void setRoot(const Logger::ptr root); 
private:
    std::string m_name;
    LogLevel::Level m_level;
    LogFormatter::ptr m_formater;
    //MutexType m_mutex;
    std::list<LogAppender::ptr> m_appenders;
    Logger::ptr m_root;
};

class StdoutAppender : public LogAppender{
public:
    typedef std::shared_ptr<StdoutAppender> ptr;
    void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override;
    std::string toYamlString() override;
};

class FileAppender : public LogAppender{
public:
    typedef std::shared_ptr<FileAppender> ptr;
    FileAppender(const std::string& filename);
    void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override;
    std::string toYamlString() override;    
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime = 0;
};

class LogManager{
public:
    typedef Spinlock MutexType;
    LogManager();
    Logger::ptr getLogger(const std::string& name);
    Logger::ptr getRoot() const {return m_root;}
    std::string toYAMLString();
private:
    MutexType m_mutex;
    std::map<std::string,Logger::ptr> m_loggers;
    Logger::ptr m_root;
};

typedef SingleTon<LogManager>  LogMgr;

}

#endif