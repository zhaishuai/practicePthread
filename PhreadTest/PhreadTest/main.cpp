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
    for(int i = 0; i < 100000  ; i++){
    threadPool::Thread thread;
        thread.startThread([]{
            printf("hello\n");
        });
    }
    sleep(100);
    
    return 0;
}
