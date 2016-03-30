//
//  main.cpp
//  PhreadTest
//
//  Created by shuaizhai on 3/23/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "learnPthreadCreate.hpp"
#include "condition.hpp"
#include "ThreadPool.hpp"
#include <list>
#include <time.h>

#include "ctpl_stl.h"

int main(int argc, const char * argv[]) {
    time_t t_start, t_end;
    t_start = time(NULL) ;
//    create::run();
    
//    condition::run();
    
//    for(int i = 0; i < 100000  ; i++){
//        
//        threadPool::Thread thread;
//        thread.startThread([&i, &thread]{
//            
////            thread.stopThread();
////            sleep(1);
//            printf("hello ,%d\n", i);
//            
//        });
//    }
    /*
    for( int j = 0; j  < 100 ; j++){
    
    threadPool::ThreadPool threadPool;

    for(int i = 0; i < 4000000; i++)
    threadPool.run([i]{
        printf("@@@@@@@@@@@@:%d\n", i);
        
        struct timeval delay;
        delay.tv_sec = 0;
        delay.tv_usec = 10 * 1000; // 20 ms
        select(0, NULL, NULL, NULL, &delay);
        
    });
    
    
    threadPool.allTaskFinished([&threadPool, &t_end, &t_start]{
        t_end = time(NULL) ;
        printf("time: %.0f s\n", difftime(t_end,t_start)) ;
        printf("all finished!\n");
//        printf("idle quantity: %ld\n", threadPool.idleQueue->size());
//        printf("task quantity: %ld\n", threadPool.taskQueue->size());
//        printf("work quantity: %ld\n", threadPool.workQueue->size());
    });
    printf("main\n");
}
    */
    
//    sleep(5);
 
    
//    while (true) {
//        
//        printf("idle quantity: %ld\n", threadPool.idleQueue->size());
//        printf("task quantity: %ld\n", threadPool.taskQueue->size());
//        printf("work quantity: %ld\n", threadPool.workQueue->size());
//        
//        sleep(1);
//    }
    
    
//    
//    for(int i = 0 ; i < 1000000; i++){
//        printf("%ld\n", sizeof(std::function<void ()>));
//    }

    
//    std::deque<double> queue;
//    
//    for(int i = 0 ; i < 4000000; i++){
////        std::function<void ()> func;
//        double func = 10;
////        func = []{printf("hello\n");};
//        queue.push_back(func);
//    }
//    queue.clear();
//    queue.shrink_to_fit();
////    std::deque<double>(queue).swap(queue);
//    
//    while (true) {
////        std::deque<double>(queue).swap(queue);
////        printf("ni hao sizeOfQueue:%ld !\n", queue.size());
//        sleep(1);
//    }

    
//    ctpl::thread_pool p(150 /* two threads in the pool */);
//    for(int i = 0; i < 4000000; i++){
//        p.push([i](int id){
//            printf("@@@@@@@@@@@@:%d\n", i);
//        });
//    }
    
    
    
//    t_end = time(NULL) ;
//    printf("time: %.0f s\n", difftime(t_end,t_start)) ;
//    
//    for(int i = 0 ; i < 10000; i++){
////
//    threadPool::Timer timer(10);
//    int index = 0;
//    timer.start([&index, &timer]{
//        if(index>=10){
//            timer.stop();
//        }
//        index++;
//        printf("start\n");
//    });
//    }

//    while (true) {
//////        struct timeval delay;
//////        delay.tv_sec = 0;
//////        delay.tv_usec = 0 * 1000; // 20 ms
//////        select(0, NULL, NULL, NULL, &delay);
//////        sleep(1);
//        usleep(1000 * 100);
//////        printf("start\n");
//    }
    
    return 0;
}
