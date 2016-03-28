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

int main(int argc, const char * argv[]) {
    
//    create::run();
    
//    condition::run();
    
//    for(int i = 0; i < 100000  ; i++){
//        
//        threadPool::Thread thread;
//        thread.startThread([&i]{
//            
//            printf("hello ,%d\n", i-1);
//        });
//    }
    
    threadPool::ThreadPool threadPool;

    for(int i = 0; i < 4000000; i++)
    threadPool.run([i]{
        printf("@@@@@@@@@@@@:%d\n", i);
        
//        struct timeval delay;
//        delay.tv_sec = 0;
//        delay.tv_usec = 10 * 1000; // 20 ms
//        select(0, NULL, NULL, NULL, &delay);
        
    });
    
    
    threadPool.allTaskFinished([&threadPool]{
        printf("all finished!\n");
        printf("idle quantity: %ld\n", threadPool.idleQueue->size());
        printf("task quantity: %ld\n", threadPool.taskQueue->size());
        printf("work quantity: %ld\n", threadPool.workQueue->size());
    });
    printf("main\n");
//    sleep(5);
 
    
    while (true) {
        
        printf("idle quantity: %ld\n", threadPool.idleQueue->size());
        printf("task quantity: %ld\n", threadPool.taskQueue->size());
        printf("work quantity: %ld\n", threadPool.workQueue->size());
        
        sleep(1);
    }
    
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
//        printf("ni hao sizeOfQueue:%ld !\n", queue.size());
//        sleep(1);
//    }


    
    return 0;
}
