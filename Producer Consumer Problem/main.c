//
//  main.c
//  Producer Consumer Problem
//
//  Created by Devang Papinwar on 29/09/21.
//

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

sem_t empty;
sem_t full;

int item = 69;
int threads = 4;

pthread_mutex_t mutex;

int p_index = 0;
int c_index = 0;
int arr[4];


// MARK: Producer Rep :
void * produce(int *no)
{
    int item;
    
    for(int i = 0 ; i < threads ; i++)
    {
        item = rand() & 5;
        
        // decrements the semaphore value passed . if val is greater than 0 then decrements and returns back
        sem_wait(&empty);
        
        // lock the thread
        pthread_mutex_lock(&mutex);
        
        arr[p_index] = item;

        printf("\nProducer %d produced value : %d " , *((int *)no) , arr[p_index]);
        
        p_index  = (p_index + 1 ) % threads;
        
        // mutex shall be locked , if already called then thread shall be blocked until the mutex becomes available
        pthread_mutex_unlock(&mutex);
        
        // increments the semaphore , if value becomes greater than 0 then blocked process will be woken up and locks the semaphore
        sem_post(&full);
    }
    printf("\n");
    return p_index;
}


// MARK: Consumer Rep :
void * consume(void *no)
{
    int index = (int)no-1;
    
    // decrements the semaphore value passed . if val is greater than 0 then decrements and returns back
    sem_wait(&full);
    
    // lock the thread
    pthread_mutex_lock(&mutex);
    
    arr[c_index] = arr[c_index] * 3;
    int item = arr[c_index];
    printf("\nConsumer %d at Item : %d " , *((int *)no) , arr[c_index] );
    c_index = (c_index + 1 ) % threads;
    
    // mutex shall be locked , if already called then thread shall be blocked until the mutex becomes available
    pthread_mutex_unlock(&mutex);
    
    // increments the semaphore , if value becomes greater than 0 then blocked process will be woken up and locks the semaphore
    sem_post(&empty);
    return c_index;
}

int main()
{
    printf("\n\nProducer Consumer Problem : \n");
    
    int threads = 4;
    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    
    pthread_t pThread[threads];
    
    // pthread created
    pthread_t prod ,cons1 ,cons2;
    
    // Initialise the Mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Initialise the semaphores with semaphore , shareable threads and initial value for semaphore
    sem_init(&empty,0,1);
    sem_init(&full,0,0);

    // create threads producer and consumer by passing the threads , default attribute intialisation
    pthread_create(&prod , NULL, (void *)produce, (void *)&a[0]);

    // waits for the thread to terminate without copying the exit status of thread
    pthread_join(prod, NULL);
    
//    pthread_create(&cons1 , NULL, (void *)consume, (void *)&a[0]);
//    pthread_create(&cons2 , NULL, (void *)consume, (void *)&a[1]);
//
//    // waits for the thread to terminate without copying the exit status of thread
//    pthread_join(cons1, NULL);
//    pthread_join(cons2, NULL);
    
    
    for(int i = 0 ; i < threads ; i++)
    {
        pthread_create(&pThread[i] , NULL , (void *)consume , (void *)&a[i]);
    }

    for(int i = 0 ; i < threads ; i++)
    {
        pthread_join(&pThread[i] , NULL);
    }

    // mutex destroyed
    pthread_mutex_destroy(&mutex);
    
    // destroy semaphore
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
