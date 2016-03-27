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
    
    struct ThreadData{
        void *data;
        std::function<void ()> callback;
    };

    
    void *threadCallback(void *data){
        pthread_mutex_unlock(&mutex);
        ThreadData *threadData = ((ThreadData *)data);
        
//        Thread *thread = (Thread *)threadData.data;
        
        
        
        threadData->callback();
        delete threadData;
        threadData = nullptr;
        pthread_exit(0);
    }
    

    
    Thread::Thread(){
        threadInfo.mut  = PTHREAD_MUTEX_INITIALIZER;
        threadInfo.cond = PTHREAD_COND_INITIALIZER;
        
    }
    
    void Thread::startThread( std::function<void ()> func){
        pthread_mutex_lock( &mutex);
        ThreadData *threadData = new ThreadData{this, func};
        pthread_detach(threadInfo.pthread);
        threadInfo.threadId = pthread_create(&threadInfo.pthread, NULL, threadCallback, (void *)threadData);
    }
    

#pragma mark - ThreadPool
    
    
    ThreadPool::ThreadPool(){
        idleQueue = std::unique_ptr<std::queue<std::shared_ptr<Thread>>>(new std::queue<std::shared_ptr<Thread>>);
        workQueue = std::unique_ptr<std::vector<std::shared_ptr<Thread>>>(new std::vector<std::shared_ptr<Thread>>);
        taskQueue = std::unique_ptr<std::vector<std::function<void()>>>(new std::vector<std::function<void()>>);
        
        idleQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        workQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        taskQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        
        for(int i = 0 ; i < miniThreads ; i++){
            std::shared_ptr<Thread> reusedThread = std::shared_ptr<Thread>(new Thread());
            reusedThread->startThread([reusedThread, this]{
                std::shared_ptr<Thread> thread = reusedThread;
                while(true){
                    pthread_mutex_lock(&taskQueueMutex);
                    std::function<void()> func;
                    if(taskQueue->size()){
                        printf("ni hao wo lai le\n");
                        func = taskQueue->front();
                        taskQueue->erase(taskQueue->begin()+0);
                    }
                    pthread_mutex_unlock(&taskQueueMutex);
                    if(func!=nullptr)
                        func();
                    else{
                        printf("func is null\n");
                    }
                    
                    
                    
                    pthread_mutex_lock(&taskQueueMutex);
                    if(taskQueue->size()){
                        pthread_mutex_unlock(&taskQueueMutex);
                        continue;
                    }
                    pthread_mutex_unlock(&taskQueueMutex);

                    
                    pthread_mutex_lock(&idleQueueMutex);
                    idleQueue->push(thread);
                    workQueue->erase(std::remove(workQueue->begin(), workQueue->end(), thread), workQueue->end());
                    pthread_mutex_unlock(&idleQueueMutex);
                    
                    pthread_mutex_lock(&thread->threadInfo.mut);
                    pthread_cond_wait(&thread->threadInfo.cond, &thread->threadInfo.mut);
                    pthread_mutex_unlock(&thread->threadInfo.mut);
                }
            });

        }
        
//        printf("init finished\n");

    }
    
    
    void ThreadPool::run(std::function<void()> func){
        
        pthread_mutex_lock(&idleQueueMutex);
        std::shared_ptr<Thread> thread = nullptr;
        if(idleQueue->size()){
            thread = idleQueue->front();
            idleQueue->pop();
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
    
}