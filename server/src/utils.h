#ifndef __UTILS_H__
#define __UTILS_H__

#include<fstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
#include<cxxabi.h>

namespace server{

pid_t GetThreadId();

template<typename T>
const char* TypeToName(){
    static const char* name = abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,nullptr);
    return name;
}

class FSUtil{
public:
    static bool Mkdir(const std::string& dirname);
    static std::string Dirname(const std::string& filename);
    static bool OpenForWrite(std::ofstream& ofs,const std::string& filename,std::ios_base::openmode mode);
};



}

#endif