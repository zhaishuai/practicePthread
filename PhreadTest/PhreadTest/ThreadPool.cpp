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
        
//        pthread_testcancel();
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

#pragma mark - ThreadPool
    
    
    ThreadPool::ThreadPool(){
        idleQueue = std::unique_ptr<std::deque<std::shared_ptr<Thread>>>(new std::deque<std::shared_ptr<Thread>>);
        workQueue = std::unique_ptr<std::vector<std::shared_ptr<Thread>>>(new std::vector<std::shared_ptr<Thread>>);
        taskQueue = std::unique_ptr<std::deque<std::function<void()>>>(new std::deque<std::function<void ()>>);
        
        idleQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        workQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        taskQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        quitRwlock     = PTHREAD_RWLOCK_INITIALIZER;
        
        for(int i = 0 ; i < miniThreads ; i++){
            std::shared_ptr<Thread> reusedThread = std::shared_ptr<Thread>(new Thread());
            reusedThread->startThread([reusedThread, this]{
                std::shared_ptr<Thread> thread = reusedThread;
                while(true){
                    pthread_rwlock_rdlock(&quitRwlock);
                    pthread_mutex_lock(&taskQueueMutex);
                    
                    pthread_testcancel();
                    
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
                        pthread_rwlock_unlock(&quitRwlock);
                        continue;
                    }
                    pthread_mutex_unlock(&taskQueueMutex);

//                    pthread_testcancel();
                    
                    pthread_mutex_lock(&idleQueueMutex);
                    idleQueue->push_back(thread);
                    workQueue->erase(std::remove(workQueue->begin(), workQueue->end(), thread), workQueue->end());
                    
                    if(idleQueue->size() == currentThreads){
                        taskQueue->shrink_to_fit();
                        if(finishCallback != nullptr){
                            finishCallback();
                        }
                    }
                    pthread_mutex_unlock(&idleQueueMutex);
                    
//                    pthread_testcancel();
                    
                    pthread_rwlock_unlock(&quitRwlock);
                    
                    pthread_mutex_lock(&thread->threadInfo.mut);
                    pthread_cond_wait(&thread->threadInfo.cond, &thread->threadInfo.mut);
                    pthread_mutex_unlock(&thread->threadInfo.mut);
                }
            });

        }

    }
    
    
    void ThreadPool::run(std::function<void()> func){
        
        pthread_mutex_lock(&idleQueueMutex);
        std::shared_ptr<Thread> thread = nullptr;
        if(idleQueue->size()){
            thread = idleQueue->front();
            idleQueue->pop_front();
            workQueue->push_back(thread);
        }
        pthread_mutex_unlock(&idleQueueMutex);
    
        
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
    
    
    
    ThreadPool::~ThreadPool(){
        
        pthread_rwlock_wrlock(&quitRwlock);
//        pthread_mutex_lock(&idleQueueMutex);
        
        for(int i = 0 ; i < idleQueue->size(); i++){
            std::shared_ptr<Thread> thread = idleQueue->at(i);
            thread->stopThread();
//            pthread_rwlock_unlock(&quitRwlock);
        }
        for(int i = 0 ; i < workQueue->size(); i++){
            std::shared_ptr<Thread> thread = workQueue->at(i);
            thread->stopThread();
//            pthread_rwlock_unlock(&quitRwlock);
        }
        
//        pthread_mutex_unlock(&idleQueueMutex);
        pthread_rwlock_unlock(&quitRwlock);
        
    }
    
}