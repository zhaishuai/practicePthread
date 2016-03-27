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

    for(int i = 0; i < 20000; i++)
    threadPool.run([i]{
        printf("@@@@@@@@@@@@:%d\n", i);
        
        struct timeval delay;
        delay.tv_sec = 0;
        delay.tv_usec = 20 * 1000; // 20 ms
        select(0, NULL, NULL, NULL, &delay);
        
    });
    printf("main\n");
    sleep(5);
 
    printf("idle quantity: %ld\n", threadPool.idleQueue->size());
    printf("task quantity: %ld\n", threadPool.taskQueue->size());
    printf("work quantity: %ld\n", threadPool.workQueue->size());
    while (true) {
        printf("task quantity: %ld\n", threadPool.taskQueue->size());
        sleep(1);
    }
//    for(int i = 0 ; i < 1000000; i++){
//        printf("%ld\n", sizeof(std::function<void ()>));
//    }
//    
    return 0;
}
