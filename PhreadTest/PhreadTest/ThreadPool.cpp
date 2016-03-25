//
//  ThreadPool.cpp
//  PhreadTest
//
//  Created by shuaizhai on 3/25/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#include "ThreadPool.hpp"


namespace threadPool{
    
    struct ThreadData{
        void *data;
        std::function<void ()> callback;
    };

    
    void *threadCallback(void *data){
        
        ThreadData *threadData = ((ThreadData *)data);
        
//        Thread *thread = (Thread *)threadData.data;
        if(threadData->callback!=nullptr)
            threadData->callback();
        
        delete threadData;
        pthread_exit(0);
    }
    
    Thread::Thread(){
        threadInfo.mut  = PTHREAD_MUTEX_INITIALIZER;
        threadInfo.cond = PTHREAD_COND_INITIALIZER;
        
    }
    
    void Thread::startThread(const std::function<void ()> func){

        ThreadData *threadData = new ThreadData{this, func};
        pthread_detach(threadInfo.pthread);
        threadInfo.threadId = pthread_create(&threadInfo.pthread, NULL, threadCallback, (void *)threadData);
        
        
    }
    
    ThreadPool::ThreadPool(){
        
    }
    
    
}