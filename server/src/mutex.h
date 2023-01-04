#ifndef __MUTEX_H__
#define __MUTEX_H__

#include"nocopyable.h"
#include<pthread.h>
#include<semaphore.h>

namespace server{

class Semaphore : Nocopyable{
public:
    Semaphore(u_int32_t count = 0);
    ~Semaphore();
    void wait();
    void notify();
private:
    sem_t m_semaphore;
};

template<class T>
class ScopedLockImpl{
public:
    ScopedLockImpl(T& mutex)
    : m_mutex(m_mutex){
        m_mutex.lock();
        m_locked = true;
    }
    ~ScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

template<typename T>
class WriteScopedLockImpl{
public:
    WriteScopedLockImpl(T& mutex):
    m_mutex(mutex){
        m_mutex.wrlock();
        m_locked = true;
    }
    ~WriteScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.wrlock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

template<typename T>
class ReadScopedLockImpl{
public:
    ReadScopedLockImpl(T& mutex):
    m_mutex(mutex){
        m_mutex.rdlock();
        m_locked = true;
    }
    ~ReadScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.rdlock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

class Spinlock : Nocopyable{
public:

    typedef ScopedLockImpl<Spinlock> Lock;

    Spinlock(){
        pthread_spin_init(&mutex,0);
    }
    ~Spinlock(){
        pthread_spin_destroy(&mutex);
    }
    void lock(){
        pthread_spin_lock(&mutex);
    }
    void unlock(){
        pthread_spin_unlock(&mutex);
    }
private:
    pthread_spinlock_t mutex;
};

class RWLOCK : Nocopyable{
public:

    typedef WriteScopedLockImpl<RWLOCK> WriteLock;
    typedef ReadScopedLockImpl<RWLOCK> ReadLock;

    RWLOCK(){
        pthread_rwlock_init(&m_mutex,nullptr);
    }
    ~RWLOCK(){
        pthread_rwlock_destroy(&m_mutex);
    }
    void rdlock(){
        pthread_rwlock_rdlock(&m_mutex);
    }
    void wrlock(){
        pthread_rwlock_wrlock(&m_mutex);
    }
    void unlock(){
        pthread_rwlock_unlock(&m_mutex);
    }
private:
    pthread_rwlock_t m_mutex;
};

}
#endif