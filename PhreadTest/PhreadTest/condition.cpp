//
//  condition.cpp
//  PhreadTest
//
//  Created by shuaizhai on 3/24/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#include "condition.hpp"


namespace condition {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    
    pthread_cond_t cond2;
    pthread_mutex_t mutex2;
    
    void *ithread(void *data){
        while(true){
            
            printf("%s\n", (char *)data);
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
            
            pthread_mutex_lock(&mutex2);
            pthread_cond_signal(&cond2);
            pthread_mutex_unlock(&mutex2);
            
            
        }
        return NULL;
    }
    
    
    void *unlockCondThread(void *data){
        while (true) {
            printf("%s\n", (char *)data);
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            
            pthread_mutex_lock(&mutex2);
            pthread_cond_wait(&cond2, &mutex2);
            pthread_mutex_unlock(&mutex2);
            
            sleep(1);
        }
    }
    
    
    void run(){
        cond = PTHREAD_COND_INITIALIZER;
        mutex = PTHREAD_MUTEX_INITIALIZER;
        
        cond2 = PTHREAD_COND_INITIALIZER;
        mutex2 = PTHREAD_MUTEX_INITIALIZER;
        
        pthread_t thread ;
//        pthread_detach(thread);
        pthread_create(&thread, NULL, ithread, (char *)"hello world");
        
        
        pthread_t thread2;
//        pthread_detach(thread2);
        pthread_create(&thread2, NULL, unlockCondThread, (char *)"^_-  ^_^  ^_* !!!!!!!");
        


        
        pthread_join(thread, NULL);
        pthread_join(thread2, NULL);
    }
    
}