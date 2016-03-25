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
    
    class Thread{
    protected:
        
    
    public:
        bool shouldQuit = false;
        ThreadInfo threadInfo;
        Thread();
        
        void startThread(std::function<void ()> func);
    
    };
    
    class ThreadPool{
    public:
        int miniThreads = 5;
        int maxThreads  = 25;
        
        std::unique_ptr<std::queue<Thread>> idleQueue;
        
        std::unique_ptr<std::queue<Thread>> workQueue;
        
        ThreadPool();
        
    protected:
        
    };
    
    
}

#endif /* ThreadPool_hpp */