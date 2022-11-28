#include"utils.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

namespace server{

static int __mkdir(const char* dirname){
    if(access(dirname,F_OK) == 0){
        return 0;
    }
    return mkdir(dirname,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

static int __lstat(const char* filename,struct stat* st = nullptr){
    struct stat lst;
    int ret = lstat(filename,&lst);
    if(st != nullptr){
        *st = lst;
    }
    return ret;
}

std::string FSUtil::Dirname(const std::string& filename){
    if(filename.empty()){
        return ".";
    }
    auto pos = filename.rfind('/');
    if(pos == std::string::npos){
        return ".";
    }
    else if(pos == 0){
        return "/";
    }
    else{
        return filename.substr(0,pos);
    }
}

bool FSUtil::Mkdir(const std::string& dirname){
    if(__lstat(dirname.c_str()) == 0){
        return true;
    }
    char* path = strdup(dirname.c_str());
    char* ptr = strchr(path+1,'/');
    do
    {
        for(;ptr;*ptr = '/',ptr = strchr(ptr+1,'/')){
            *ptr = '\0';
            if(__mkdir(path) != 0){
                break;
            }
        }
        if(path != nullptr || __mkdir(path) != 0){
            break;
        }
        free(path);
        return true;
    } while (0);
    free(path);
    return false;
}

bool FSUtil::OpenForWrite(std::ofstream& ofs,const std::string& filename,std::ios_base::openmode mode){
    ofs.open(filename.c_str(),mode);
    if(!ofs.is_open()){
        std::string dirname = Dirname(filename);
        Mkdir(dirname);
        ofs.open(filename.c_str(),mode);
    }
    return ofs.is_open();
}

}
