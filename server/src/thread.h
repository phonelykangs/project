#ifndef __THREAD_H__
#define __THREAD_H__

#include<pthread.h>
#include<functional>
#include<memory>
#include<string>
#include"nocopyable.h"

namespace server{

class Thread : Nocopyable{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb,const std::string& name);
    ~Thread();
private:
    pid_t m_id = -1;
    pthread_t m_thread_id = 0;
    std::function<void()> m_cb;
    std::string m_name;
};


}

#endif