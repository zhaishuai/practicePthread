//
//  learnPthreadCreate.cpp
//  PhreadTest
//
//  Created by shuaizhai on 3/24/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#include "learnPthreadCreate.hpp"

namespace create {
    
    int a = 0;
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    void *do_loop(void* data){
        int me = *(int *)data;
        
        printf("%d -Got by 'i:%d'\n", a, me-1);
        a++;
        pthread_mutex_unlock(&mutex);
        
        
        for(int i = 0; i < 900000000/(me + 1); i++);
        printf("me:%d finish\n",me-1);
        
        pthread_exit(NULL);
        return NULL;
    }
    
    void run() {
        int thread_id;
        for(int i = 0; i < 10;i++){
            pthread_mutex_lock(&mutex);
            pthread_t p_thread;
            pthread_detach(p_thread);
            thread_id = pthread_create(&p_thread, NULL, do_loop, &i);
        }
        sleep(10);
    }
}

