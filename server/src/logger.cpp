#include<stdarg.h>
#include<iostream>
#include<map>
#include<functional>
#include<time.h>
#include<tuple>
#include<yaml-cpp/yaml.h>
#include"logger.h"
#include"config.h"

namespace server{

const char* LogLevel::ToString(LogLevel::Level level){
    switch (level)
    {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";
        break;
    }
    return "UNKNOW";
}

LogLevel::Level LogLevel::FromString(const std::string& str){
#define XX(level,v) \
    if(str == #v){ \
        return LogLevel::level; \
    }   

    XX(DEBUG,debug);
    XX(INFO,info);
    XX(WARN,warn);
    XX(ERROR,error);
    XX(FATAL,fatal);

    XX(DEBUG,DEBUG);
    XX(INFO,INFO);
    XX(WARN,WARN);
    XX(ERROR,ERROR);
    XX(FATAL,FATAL);

#undef XX
    return UNKNOW;
}

LogEvent::LogEvent(std::shared_ptr<Logger> logger,const char* filename,uint32_t line, \
    uint32_t elapse,uint32_t threadId,uint32_t fiberId, \
    uint64_t time,const std::string& threadName,LogLevel::Level level):
    m_logger(logger),
    m_fileName(filename),
    m_line(line),
    m_elapse(elapse),
    m_threadId(threadId),
    m_fiberId(fiberId),
    m_time(time),
    m_threadName(threadName),
    m_level(level){
}

void LogEvent::format(const char* fmt,...){
    va_list al;
    va_start(al,fmt);
    format(fmt,al);
    va_end(al);
}

void LogEvent::format(const char* fmt,va_list al){
    char* buf = nullptr;
    int len = vasprintf(&buf,fmt,al);
    if(len != -1){
        m_ss << std::string(buf,len);
        free(buf);
    }
}

LogEventWrap::~LogEventWrap(){
    m_event->getLogger()->log(m_event->getLevel(),m_event);
}

class DataFormatItem : public LogFormatter::FormatItem{
public:
    DataFormatItem(const std::string str = "%Y-%m-%d %H:%M:%S")
    :m_string(str){
        if(m_string.empty()){
            m_string = "%Y-%m-%d %H:%M:%S";
        }
    }

    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
         time_t timestamp = event->getTime();
         struct tm tm_ins;
         localtime_r(&timestamp,&tm_ins);
         char linebuf[64];
         strftime(linebuf,sizeof(linebuf),m_string.c_str(),&tm_ins);
         os << linebuf;
    }
private:
    std::string m_string;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
public:
    FilenameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getFileName();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getThreadName();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
public:
    FiberIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getFiberId();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem{
public:
    ThreadIdFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getThreadId();
    } 
};

class NameFormatItem : public LogFormatter::FormatItem{
public:
    NameFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getLogger()->getName();
    } 
};

class ElapseFormatItem : public LogFormatter::FormatItem{
public:
    ElapseFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getElapse();
    } 
};

class LevelFormatItem : public LogFormatter::FormatItem{
public:
    LevelFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << LogLevel::ToString(level);
    } 
};

class MessageFormatItem : public LogFormatter::FormatItem{
public:
    MessageFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getContent();
    } 
};

class TabFormatItem : public LogFormatter::FormatItem{
public:
    TabFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << "\t";
    }
};

class StringFormatItem : public LogFormatter::FormatItem{
public:
    StringFormatItem(const std::string& str = "") : m_string(str){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << m_string;
    }
private:
    std::string m_string;
};

class NewLineFormatItem : public LogFormatter::FormatItem{
public:
    NewLineFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << std::endl;
    } 
};

class LineFormatItem : public LogFormatter::FormatItem{
public:
    LineFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level) override{
        os << event->getLine();
    } 
};

LogFormatter::LogFormatter(const std::string& pattern)
:m_pattern(pattern){
    init();
}

void LogFormatter::init(){
    static std::map<std::string,std::function<FormatItem::ptr(const std::string& str)> > format_items = {
#define XX(str,C)  \
    { #str,[](const std::string& fmt){return FormatItem::ptr(new C(fmt)); } }

        XX(m, MessageFormatItem),           //m:??????
        XX(p, LevelFormatItem),             //p:????????????
        XX(r, ElapseFormatItem),            //r:???????????????
        XX(c, NameFormatItem),              //c:????????????
        XX(t, ThreadIdFormatItem),          //t:??????id
        XX(n, NewLineFormatItem),           //n:??????
        XX(d, DataFormatItem),              //d:??????
        XX(f, FilenameFormatItem),          //f:?????????
        XX(l, LineFormatItem),              //l:??????
        XX(T, TabFormatItem),               //T:Tab
        XX(F, FiberIdFormatItem),           //F:??????id
        XX(N, ThreadNameFormatItem),        //N:????????????

#undef XX
    };

    std::vector<std::tuple<std::string,std::string,int> > vec;
    std::string nstr = "";
    for(size_t i = 0;i < m_pattern.size();++i){
        if(m_pattern[i] != '%'){
            nstr.append(1,m_pattern[i]);
            continue;
        }
        if((i+1) < m_pattern.size() && m_pattern[i+1] == '%'){
            nstr.append(1,'%');
            continue;
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;

        while(n < m_pattern.size()){
            if(!fmt_status && (!isalpha(m_pattern[n])) && m_pattern[n] != '{' && m_pattern[n] != '}'){
                str = m_pattern.substr(i+1,n-i-1);
                break;
            }
            if(fmt_status == 0){
                if(m_pattern[n] == '{'){
                    fmt_status = 1;
                    str = m_pattern.substr(i+1,n-i-1);
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            }
            else if(fmt_status == 1){
                if(m_pattern[n] == '}'){
                    fmt_status = 0;
                    fmt = m_pattern.substr(fmt_begin+1,n-fmt_begin-1);
                    ++n;
                    break;
                }
            }
            ++n;
            if(n == m_pattern.size()){
                if(str.empty()){
                    str = m_pattern.substr(i+1);
                }
            }
        }
        if(fmt_status == 0){
            if(!nstr.empty()){
                vec.push_back(std::make_tuple(nstr,std::string(),0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str,fmt,1));
            i = n-1;
        }
        else if(fmt_status == 1){
            std::cout << "pattren parse error : " << m_pattern << "-" << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern error>>",std::string(),0));
        }
    }
    if(!nstr.empty()){
        vec.push_back(std::make_tuple(nstr,"",0));
    }
    
    for(auto& v:vec){
        if(std::get<2>(v) == 0){
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(v))));
        }
        else{
            auto iter = format_items.find(std::get<0>(v));
            if(iter != format_items.end()){
                m_items.push_back(iter->second(std::get<1>(v)));
            }
            else{
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(v) + ">>")));
                m_error = true;
            }
        }
    }
}

LogFormatter::ptr LogAppender::getFormater(){
    MutexType::Lock l(m_mutex);
    return m_formater;
}

LogLevel::Level LogAppender::getLevel(){
    MutexType::Lock l(m_mutex);
    return m_level;
}

bool LogAppender:: hasFormater(){
    MutexType::Lock l(m_mutex);
    return m_hasFormater;
}

void LogAppender::setLevel(LogLevel::Level level){
    MutexType::Lock lock(m_mutex);
    m_level = level;
}
void LogAppender::setFormater(LogFormatter::ptr formater){
    MutexType::Lock lock(m_mutex);
    m_formater = formater;
    if(m_formater)
        m_hasFormater = true;
    else 
        m_hasFormater = false;
}

void StdoutAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    MutexType::Lock lock(m_mutex);
    if(level >= m_level){
        m_formater->format(std::cout,logger,event,level);
    }
}

std::string StdoutAppender::toYamlString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = "StdoutAppender";
    if(m_level != LogLevel::Level::UNKNOW){
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormater && m_formater){
        node["formater"] = m_formater->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level){
    std::stringstream ss;
    for(auto& item:m_items){
        item->format(ss,logger,event,level);
    }
    return ss.str();
}

std::ostream& LogFormatter::format(std::ostream& os,std::shared_ptr<Logger> logger,LogEvent::ptr event,LogLevel::Level level){
    for(auto& item:m_items){
        item->format(os,logger,event,level);
    }
    return os;
}

FileAppender::FileAppender(const std::string& filename):m_filename(filename){
    reopen();
}

void FileAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    
    if(level >= m_level){
        uint64_t now = event->getTime();
        if(now >= (m_lastTime + 3)) {
            reopen();
            m_lastTime = now;
        }
        MutexType::Lock lock(m_mutex);
        if(!m_formater->format(m_filestream,logger,event,level)){
            std::cout << "error" << std::endl;
        }
    }
}

std::string FileAppender::toYamlString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = "FileAppender";
    node["file"] = m_filename;
    if(m_level != LogLevel::Level::UNKNOW){
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormater && m_formater){
        node["formater"] = m_formater->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}  

bool FileAppender::reopen(){
    MutexType::Lock lock(m_mutex);
    if(m_filestream){
        m_filestream.close();
    }
    return FSUtil::OpenForWrite(m_filestream,m_filename,std::ios::app);
}

Logger::Logger(const std::string& name)
:m_name(name),m_level(LogLevel::Level::DEBUG){
    m_formater.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
}

void Logger::log(LogLevel::Level level,LogEvent::ptr event){
    if(level >= m_level){
        auto self = shared_from_this();
        MutexType::Lock lock(m_mutex);
        if(!m_appenders.empty()){
            for(auto& app:m_appenders){
                app->log(self,level,event);
            }
        }
        else if(m_root){
            m_root->log(level,event);
        }
    }
}

void Logger::setRoot(const Logger::ptr root){
    MutexType::Lock l(m_mutex);
    m_root = root;
}

void Logger::debug(LogEvent::ptr event){
    log(LogLevel::Level::DEBUG,event);
}

void Logger::info(LogEvent::ptr event){
    log(LogLevel::Level::INFO,event);
}

void Logger::warn(LogEvent::ptr event){
    log(LogLevel::Level::WARN,event);
}

void Logger::error(LogEvent::ptr event){
    log(LogLevel::Level::ERROR,event);
}

void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::Level::FATAL,event);
}

void Logger::setFormater(const std::string& pattern){
    LogFormatter::ptr newFormater = std::make_shared<LogFormatter>(pattern);
    if(newFormater->isError()){
        std::cout << __FILE__  << " : "<< __LINE__ << std::endl;
        std::cout << "Logger setFormatter name=" << m_name
                  << " value=" << pattern << " invalid formatter"
                  << std::endl;
        return;
    }
    setFormater(newFormater);
}

void Logger::setFormater(const LogFormatter::ptr formater){
    MutexType::Lock lock(m_mutex);
    m_formater = formater;
    
    for(auto& app:m_appenders){
        if(!app->hasFormater()){
            app->setFormater(formater);
        }
    }
}

void Logger::addAppender(const LogAppender::ptr appender){
    MutexType::Lock l(m_mutex);
    if(!appender->hasFormater()){
        appender->setFormater(m_formater);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(const LogAppender::ptr appender){
    MutexType::Lock l(m_mutex);
    for(auto iter = m_appenders.begin();iter != m_appenders.end();++iter){
        if(*iter == appender){
            m_appenders.erase(iter);
            break;
        }
    }
}

LogLevel::Level Logger::getLevel(){
    MutexType::Lock lock(m_mutex);
    return m_level;
}

void Logger::setLevel(LogLevel::Level level){
    MutexType::Lock lock(m_mutex);
    m_level = level;
}

void Logger::clearAppenders(){
    MutexType::Lock lock(m_mutex);
    m_appenders.clear();
}

std::string Logger::toYamlString(){
    MutexType::Lock l(m_mutex);
    YAML::Node node;
    node["name"] = m_name;
    if(m_level != LogLevel::Level::UNKNOW){
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_formater){
        node["formater"] = m_formater->getPattern();
    }
    for(auto& app: m_appenders){
        node["appenders"].push_back(YAML::Load(app->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}
LogFormatter::ptr Logger::getFormater(){
    MutexType::Lock lock(m_mutex);
    return m_formater;
}

LogManager::LogManager(){
    m_root.reset(new Logger());
    m_root->addAppender(std::dynamic_pointer_cast<LogAppender>(std::make_shared<StdoutAppender>()));
    m_loggers[m_root->getName()] = m_root;
}

Logger::ptr LogManager::getLogger(const std::string& name){
    MutexType::Lock l(m_mutex);
    auto iter = m_loggers.find(name);
    if(iter != m_loggers.end()){
        return iter->second;
    }

    Logger::ptr logger= std::make_shared<Logger>(name);
    m_loggers[name] = logger;
    logger->setRoot(m_root);
    return logger;
}

std::string LogManager::toYAMLString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    for(auto iter = m_loggers.begin();iter != m_loggers.end();++iter){
        node.push_back(YAML::Load(iter->second->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

struct LogAppenderDefine{
    int type = 0; // 1:FileAppender 2:StdoutAppender
    std::string m_formater;
    LogLevel::Level m_level = LogLevel::Level::UNKNOW;
    std::string m_file;

    bool operator==(const LogAppenderDefine& oth) const {
        return this->type == oth.type 
        && this->m_file == oth.m_file 
        && this->m_formater == oth.m_formater 
        && this->m_level == oth.m_level;
    }
};


struct LogDefine{
    std::string m_name;
    LogLevel::Level m_level = LogLevel::Level::UNKNOW;
    std::string m_formater;
    std::vector<LogAppenderDefine> m_appenders;

    bool operator==(const LogDefine& oth) const {
        return this->m_appenders == oth.m_appenders
        && this->m_name == oth.m_name
        && this->m_level == oth.m_level
        && this->m_formater == oth.m_formater; 
    }

    bool operator<(const LogDefine& oth) const {
        return this->m_name < oth.m_name;
    }
};

template<>
class LexicalCast<std::string,LogDefine>{
public:
    LogDefine operator()(const std::string& str){
        YAML::Node node = YAML::Load(str);
        LogDefine ld;
        if(!node["name"].IsDefined()){
            std::cout << "log config error, name not exist!" << std::endl;
            throw std::logic_error("log config name is null"); 
        }
        ld.m_name = node["name"].as<std::string>();
        ld.m_level = server::LogLevel::FromString(node["level"].IsDefined() ? node["level"].as<std::string>() : "");
        if(node["formater"].IsDefined()){
            ld.m_formater = node["formater"].as<std::string>();
        }
        if(node["appenders"].IsDefined()){
            for(auto iter = node["appenders"].begin();iter != node["appenders"].end();++iter){
                if(!(*iter)["type"].IsDefined()){
                    std::cout << "appender type is null!" << std::endl;
                    continue;
                }   
                std::string type = (*iter)["type"].as<std::string>();
                LogAppenderDefine lad;
                lad.m_level = LogLevel::FromString((*iter)["type"].IsDefined() ? (*iter)["type"].as<std::string>() : "");
                if(type == "FileAppender"){
                    lad.type = 1;
                    if(!(*iter)["file"].IsDefined()){
                        std::cout << "FileAppender file name is null!" << std::endl;
                    }
                    lad.m_file = (*iter)["file"].as<std::string>();
                    if((*iter)["formater"].IsDefined()){
                        lad.m_formater = (*iter)["formater"].as<std::string>();
                    }
                }
                else if(type == "StdAppender"){
                    lad.type = 2;
                    if((*iter)["formater"].IsDefined()){
                        lad.m_formater = (*iter)["formater"].as<std::string>();
                    }
                }
                else
                    std::cout << "Appender type is null!" << std::endl;
                ld.m_appenders.push_back(lad);
            }
        }
        return ld;
    }
};

template<>
class LexicalCast<LogDefine,std::string>{
public:
    std::string operator()(const LogDefine& ld){
        YAML::Node node;
        node["name"] = ld.m_name;
        node["level"] = ld.m_level == LogLevel::Level::UNKNOW ? "" : LogLevel::ToString(ld.m_level);
        node["formater"] = ld.m_formater;
        for(auto& app:ld.m_appenders){
            YAML::Node a;
            if(app.type == 1){
                a["type"] = "FileAppender";
                a["file"] = app.m_file;
            }
            else if(app.type == 2){
                a["type"] = "StdAppender";
            }
            a["level"] = app.m_level == LogLevel::Level::UNKNOW ? "" : LogLevel::ToString(app.m_level);
            a["formater"] = app.m_formater;
            node["appenders"].push_back(a);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

server::ConfigVar<std::set<LogDefine> >::ptr g_logger_config =
 server::Config::lookup("logs",std::set<LogDefine>(),"Logs Config register");

struct LogIniter{
    LogIniter(){
        g_logger_config->addCallBackFunc([](const std::set<LogDefine>& oldvalue,const std::set<LogDefine>& newvalue){
            LOG_INFO(GET_LOG_ROOT()) << "callback function: logger config changed!";
            for(auto& ld:newvalue){
                auto iter = oldvalue.find(ld);
                server::Logger::ptr logger;
                if(iter == oldvalue.end()){
                    logger = GET_LOG_NAME(ld.m_name);
                }
                else{
                    if(!(*iter == ld)){
                        logger = GET_LOG_NAME(ld.m_name);
                    }
                    else
                        continue;
                }
                if(ld.m_level != LogLevel::Level::UNKNOW)
                    logger->setLevel(ld.m_level);
                if(!ld.m_formater.empty()){
                    logger->setFormater(ld.m_formater);
                }
                logger->clearAppenders();
                for(auto& app:ld.m_appenders){
                    server::LogAppender::ptr ap;
                    if(app.type == 1){
                        ap = std::make_shared<FileAppender>(app.m_file);
                    }
                    else if(app.type == 2){
                        ap = std::make_shared<StdoutAppender>();
                    }
                    if(app.m_level != LogLevel::Level::UNKNOW)
                        ap->setLevel(app.m_level);
                    if(!app.m_formater.empty()){
                        server::LogFormatter::ptr formater = std::make_shared<LogFormatter>(app.m_formater);
                        if(!formater->isError())
                            ap->setFormater(formater);
                        else
                            std::cout << "log.name=" << ld.m_name << " appender type=" << app.type
                                      << " formatter=" << app.m_formater << " is invalid" << std::endl;
                    }
                    logger->addAppender(ap);
                }
                for(auto& ld:oldvalue){
                    auto iter = newvalue.find(ld);
                    if(iter == newvalue.end()){
                        server::Logger::ptr logger = GET_LOG_NAME(ld.m_name);
                        logger->setLevel(LogLevel::Level::UNKNOW);
                        logger->clearAppenders();
                    }
                }
            }
        });
    }
};

static LogIniter __s_log_initer;

}