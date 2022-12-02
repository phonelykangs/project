#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace server{

template<typename T>
class SingleTon{
public:
    static T* GetInstance(){
        static T value;
        return &value;
    }

};

}

#endif