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
    

    
    
    
    ThreadPool::ThreadPool(){
        idleQueue = std::unique_ptr<std::queue<Thread>>(new std::queue<Thread>);
        for(int i = 0 ; i < miniThreads ; i++){
            
        }
        
        workQueue = std::unique_ptr<std::queue<Thread>>(new std::queue<Thread>);
        
    }
    
    
}