#ifndef __LOGLEVEL_H__
#define __LOGLEVEL_H__

#include<string>

namespace server{

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

}

#endif