#ifndef __UTILS_H__
#define __UTILS_H__

#include<fstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

namespace server{

class FSUtil{
public:
    static bool Mkdir(const char* path);
    static std::string Dirname(const std::string& filename);
    static bool OpenForWrite(std::ofstream& ofs,const std::string& filename,std::ios_base::openmode mode);
};



}

#endif