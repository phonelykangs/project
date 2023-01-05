#include"server.h"
#include"vector"
#include<memory>

static server::Logger::ptr g_logger = GET_LOG_ROOT();
volatile int count = 0;
server::CASLock s_mutex;

void func1(){
    LOG_INFO(g_logger) << "name: " << server::Thread::GetName()
                                                << " this.name: " << server::Thread::GetThis()->getName() 
                                                << " id : " << server::GetThreadId()
                                                << " this.id " << server::Thread::GetThis()->getId();
    pthread_exit(nullptr);
}

void func2(){  // for lock test
    for(int i = 0;i < 10000000;++i){
        s_mutex.lock();
        ++count;
        s_mutex.unlock();
    }
}

int main(){
    std::vector<server::Thread::ptr> thrs;
    for(int i = 0;i < 5;++i){
        server::Thread::ptr thr = std::make_shared<server::Thread>(&func2,"func" + std::to_string(i));
        thrs.push_back(thr);
    }
    for(int i = 0;i < 5;++i){
        thrs[i]->join();
    }
    LOG_INFO(g_logger) << count;
}