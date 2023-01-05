#include"thread.h"
#include"string.h"
#include"logger.h"
#include"utils.h"

namespace server{

static thread_local Thread* tl_thread = nullptr;
static thread_local std::string tl_name = "UNKNOWN";

static server::Logger::ptr g_logger = GET_LOG_NAME("system");

Thread::Thread(std::function<void()> cb,const std::string& name)
: m_cb(cb)
, m_name(name){
    if(name.empty()){
        m_name = "UNKNOWN";
    }
    int ret = pthread_create(&m_thread_id,nullptr,&Thread::run,this);
    if(ret){
        LOG_ERROR(g_logger) << "pthread_create error:" << " name = " << name << 
        " error = " << ret << " " << strerror(ret);
        throw std::logic_error("pthread_create error");
    }
    m_semaphore.wait();
}

Thread::~Thread(){
    if(m_thread_id){
        pthread_detach(m_thread_id);
    }
}

void Thread::join(){
    if(m_thread_id){
        int ret = pthread_join(m_thread_id,nullptr);
        if(ret){
            LOG_ERROR(g_logger) << "pthread_join error:" << 
            " error = " << ret << " " << strerror(ret);
            throw std::logic_error("pthread_join error");
        }
        m_thread_id = 0;
    }
}

Thread* Thread:: GetThis(){
    return tl_thread;
}

const std::string& Thread::GetName(){
    return tl_name;
}

void Thread::SetName(const std::string& name){
    if(name.empty()){
        return ;
    }
    if(tl_thread){
        tl_thread->m_name = name;
    }
    tl_name = name;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}

void* Thread::run(void* arg){
    Thread* thread = (Thread*)arg;
    tl_thread = thread;
    tl_name = thread->m_name;
    thread->m_id = GetThreadId();
    pthread_setname_np(pthread_self(),thread->m_name.substr(0,15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb);

    thread->m_semaphore.notify();
    cb();
    return 0;
}

}
