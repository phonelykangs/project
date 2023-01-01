#ifndef __THREAD_H__
#define __THREAD_H__

#include<pthread.h>
#include<functional>
#include"nocopyable.h"

namespace server{

class Thread : Nocopyable{
public:
private:
    std::function<void()> m_cb;
};


}

#endif