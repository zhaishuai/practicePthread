//
//  ThreadPool.cpp
//  PhreadTest
//
//  Created by shuaizhai on 3/25/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#include "ThreadPool.hpp"


namespace threadPool{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    
    void *threadCallback(void *data){
        pthread_mutex_unlock(&mutex);
        ThreadData *threadData = ((ThreadData *)data);
        
        threadData->callback();
        pthread_exit(0);
    }
    

    
    Thread::Thread(){
        threadInfo.mut  = PTHREAD_MUTEX_INITIALIZER;
        threadInfo.cond = PTHREAD_COND_INITIALIZER;
        
    }
    
    void Thread::startThread( std::function<void ()> func){
        pthread_mutex_lock( &mutex);
        threadData = std::unique_ptr<ThreadData>(new ThreadData{this, func});
        pthread_detach(threadInfo.pthread);
        threadInfo.threadId = pthread_create(&threadInfo.pthread, NULL, threadCallback, (void *)threadData.get());
    }
    
    void Thread::stopThread(){
        pthread_cancel(threadInfo.pthread);
    }

#pragma mark - Timer
    
    Timer::Timer(int timeInterval){
        thread = std::unique_ptr<Thread>(new Thread);
        this->delaytime = 0;
        this->timeInterval = timeInterval;
        mut = PTHREAD_MUTEX_INITIALIZER;
    }
    
    Timer::Timer(int delay, int timeInterval){
        thread = std::unique_ptr<Thread>(new Thread);
        this->delaytime = delay;
        this->timeInterval = timeInterval;
        mut = PTHREAD_MUTEX_INITIALIZER;
        
    }
    
    void Timer::start(std::function<void ()> func){
        timeStarted = true;
        thread->startThread([&func, this]{
            
            while (true) {
                usleep(this->delaytime*1000);
                if(func!=nullptr){
                    func();
                }
                usleep(this->timeInterval*1000);
                pthread_mutex_lock(&mut);
                if(!timeStarted){
                    break;
                }
                pthread_mutex_unlock(&mut);
            }
            
        });
    }
    
    void Timer::stop(){
        pthread_mutex_lock(&mut);
        timeStarted = false;
        pthread_mutex_unlock(&mut);
    }
    
//    Timer::~Timer(){
//        stop();
//        while (size != 0) {
//            sched_yield();
//        }
//    }
    
#pragma mark - ThreadPool
    
    ThreadPool::ThreadPool(){
        idleQueue = std::unique_ptr<std::deque<std::shared_ptr<Thread>>>(new std::deque<std::shared_ptr<Thread>>);
        workQueue = std::unique_ptr<std::vector<std::shared_ptr<Thread>>>(new std::vector<std::shared_ptr<Thread>>);
        taskQueue = std::unique_ptr<std::deque<std::function<void()>>>(new std::deque<std::function<void ()>>);
        
        threadQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        taskQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        
        for(int i = 0 ; i < miniThreads ; i++){
            addThreadIntoPool();
        }

    }
    
    void ThreadPool::addThreadIntoPool(){
        std::shared_ptr<Thread> reusedThread = std::shared_ptr<Thread>(new Thread());
        reusedThread->startThread([reusedThread, this]{
            std::shared_ptr<Thread> thread = reusedThread;
            while(true){
                pthread_testcancel();
                pthread_mutex_lock(&taskQueueMutex);
                std::function<void()> func;
                if(taskQueue->size()){
                    func = taskQueue->front();
                    taskQueue->pop_front();
                }
                pthread_mutex_unlock(&taskQueueMutex);
                if(func!=nullptr)
                    func();
                
                pthread_mutex_lock(&taskQueueMutex);
                if(taskQueue->size()){
                    pthread_mutex_unlock(&taskQueueMutex);
                    continue;
                }
                pthread_mutex_unlock(&taskQueueMutex);
                
                pthread_mutex_lock(&threadQueueMutex);
                idleQueue->push_back(thread);
                workQueue->erase(std::remove(workQueue->begin(), workQueue->end(), thread), workQueue->end());
                
                if(idleQueue->size() == currentThreads){
                    taskQueue->shrink_to_fit();
                    if(finishCallback != nullptr){
                        finishCallback();
                    }
                }
                pthread_mutex_unlock(&threadQueueMutex);
                
                pthread_mutex_lock(&thread->threadInfo.mut);
                pthread_cond_wait(&thread->threadInfo.cond, &thread->threadInfo.mut);
                pthread_mutex_unlock(&thread->threadInfo.mut);
            }
        });
    }
    
    void ThreadPool::run(std::function<void()> func){
        
        pthread_mutex_lock(&threadQueueMutex);
        std::shared_ptr<Thread> thread = nullptr;
        if(idleQueue->size()){
            thread = idleQueue->front();
            idleQueue->pop_front();
            workQueue->push_back(thread);
        }
        pthread_mutex_unlock(&threadQueueMutex);
    
        
        pthread_mutex_lock(&taskQueueMutex);
        taskQueue->push_back(func);
        pthread_mutex_unlock(&taskQueueMutex);
        
        if(thread){
            pthread_mutex_lock(&thread->threadInfo.mut);
            pthread_cond_signal(&thread->threadInfo.cond);
            pthread_mutex_unlock(&thread->threadInfo.mut);
        }
        
    }
    
    void ThreadPool::allTaskFinished(std::function<void ()> func){
        finishCallback = func;
    }
    
    void ThreadPool::stop(){
        pthread_mutex_lock(&taskQueueMutex);
        taskQueue->clear();
        pthread_mutex_unlock(&taskQueueMutex);
        
        long size = 1;
        
        while (size != 0) {
            pthread_mutex_lock(&threadQueueMutex);
            size = workQueue->size();
            sched_yield();
            pthread_mutex_unlock(&threadQueueMutex);
        }
    }
    
    ThreadPool::~ThreadPool(){
        
        stop();
        
        pthread_mutex_lock(&threadQueueMutex);
        for(int i = 0 ; i < idleQueue->size(); i++){
            std::shared_ptr<Thread> thread = idleQueue->at(i);
            thread->stopThread();

        }
        for(int i = 0 ; i < workQueue->size(); i++){
            std::shared_ptr<Thread> thread = workQueue->at(i);
            thread->stopThread();

        }
        pthread_mutex_unlock(&threadQueueMutex);
        
    }
    
}