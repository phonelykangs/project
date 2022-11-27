#include"logger.h"
#include<stdarg.h>
#include<iostream>
#include<map>
#include<functional>
#include<time.h>
#include<tuple>

namespace server{

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
    m_threadName(threadName){
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

        XX(m, MessageFormatItem),           //m:消息
        XX(p, LevelFormatItem),             //p:日志级别
        XX(r, ElapseFormatItem),            //r:累计毫秒数
        XX(c, NameFormatItem),              //c:日志名称
        XX(t, ThreadIdFormatItem),          //t:线程id
        XX(n, NewLineFormatItem),           //n:换行
        XX(d, DataFormatItem),              //d:时间
        XX(f, FilenameFormatItem),          //f:文件名
        XX(l, LineFormatItem),              //l:行号
        XX(T, TabFormatItem),               //T:Tab
        XX(F, FiberIdFormatItem),           //F:协程id
        XX(N, ThreadNameFormatItem),        //N:线程名称

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

}