#include"server.h"
#include"vector"
#include<memory>

server::Logger::ptr g_logger = GET_LOG_NAME("system");
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
    for(int i = 0;i < 10000;++i){
        s_mutex.lock();
        ++count;
        s_mutex.unlock();
    }
}

void func3(){
    while (1){
        LOG_INFO(g_logger) << "XXXXXXXXXXXXXXXXXXXXXXX";
    }
}

void func4(){
    while (1){
        LOG_INFO(g_logger) << "-------------------------";
    }
}

int main(){
    YAML::Node node = YAML::LoadFile("/home/phonelykangs/project/project/server/test/log.yml");
    server::Config::LoadFromYaml(node);
    std::cout << server::LogMgr::GetInstance()->toYAMLString();
    std::vector<server::Thread::ptr> thrs;
    for(int i = 0;i < 5;++i){
        server::Thread::ptr thr1 = std::make_shared<server::Thread>(&func2,"func" + std::to_string(i));
        server::Thread::ptr thr2 = std::make_shared<server::Thread>(&func2,"func" + std::to_string(i));
        thrs.push_back(thr1);
        thrs.push_back(thr2);
    }
    
    for(size_t i = 0;i < thrs.size();++i){
        thrs[i]->join();
    }
    server::Config::visit([](server::ConfigVarBase::ptr p){
        LOG_INFO(g_logger) << " name: " << p->getName()
                            << " description: " << p->getDescription()
                            << " type: " << p->getTypeName()
                            << " value " << p->toString();
    });
    LOG_INFO(g_logger) << count;
}