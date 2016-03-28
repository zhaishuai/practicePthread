//
//  ThreadPool.hpp
//  PhreadTest
//
//  Created by shuaizhai on 3/25/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <err.h>
#include <vector>
#include <functional>
#include <queue>

namespace threadPool{
    
    struct ThreadInfo{
        pthread_mutex_t mut;
        pthread_cond_t  cond;
        pthread_t       pthread;
        int             threadId;
    };
    
    struct ThreadData{
        void *data;
        std::function<void ()> callback;
    };

    
    class Thread{
    protected:
        std::unique_ptr<ThreadData> threadData;
    
    public:
        bool shouldQuit = false;
        ThreadInfo threadInfo;
        
        Thread();
        
        void startThread(std::function<void ()> func);
        
        void stopThread();
    
    };
    
    class ThreadPool{
    public:
        int miniThreads = 150;
        int maxThreads  = 500;
        
        pthread_mutex_t idleQueueMutex;
        std::unique_ptr<std::deque<std::shared_ptr<Thread>>> idleQueue;
        
        pthread_mutex_t workQueueMutex;
        std::unique_ptr<std::vector<std::shared_ptr<Thread>>> workQueue;
        
        pthread_mutex_t taskQueueMutex;
        std::unique_ptr<std::deque<std::function<void()>>> taskQueue;
        
        ThreadPool();
        ~ThreadPool();
        
        void run(std::function<void()> func);
        
        void allTaskFinished(std::function<void ()> func);
        
    protected:
        int currentThreads = miniThreads;
        
        std::function<void ()> finishCallback;
        
    };
    
    
}

#endif /* ThreadPool_hpp */
