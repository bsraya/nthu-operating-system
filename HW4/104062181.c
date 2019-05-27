#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

typedef struct _Index {
    int index;
} Index;

// global variables
#define PHILOSOPHER_NUMBER 5
#define LEFT (nth_philosopher + PHILOSOPHER_NUMBER - 1) % PHILOSOPHER_NUMBER
#define RIGHT (nth_philosopher + 1) % PHILOSOPHER_NUMBER
#define DURATION (rand() % 3 + 1)
static enum{THINKING, HUNGRY, EATING} state[5];
static pthread_cond_t cond_var[PHILOSOPHER_NUMBER];
static pthread_mutex_t mutex;

// prototypes
void thinking(int);
void pickup_forks(int);
void return_forks(int);
void test(int);
void *philosopher(void *);

int main() {
    int i=0;

    pthread_mutex_init(&mutex, NULL);
    
    //initialization
    for(i=0; i<PHILOSOPHER_NUMBER; i++) {
        pthread_cond_init(cond_var+1, NULL); // condition variable with mutex lock
        state[i] = THINKING;
    }

    pthread_t thread[PHILOSOPHER_NUMBER]; // create five threads
    Index* thread_index[PHILOSOPHER_NUMBER]; // put each thread in each index
    
    // to start a thread for each philosopher
    for(i=0; i<PHILOSOPHER_NUMBER; i++) {
        thread_index[i] = (Index*)malloc(sizeof(Index*));
        thread_index[i]->index = i;
    }

    // to control the philosophers' actions
    for(i=0; i<PHILOSOPHER_NUMBER; i++) {
        pthread_create(thread+1, NULL, philosophers, (void*)(thread_index[i]));
    }

    // to join the philosophers' threads in order
    for(i=0; i<PHILOSOPHER_NUMBER; i++) {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

void thinking(int nth_philosopher) {
    printf("Philosoper %d is now THINKING for %d seconds.\n", nth_philosopher, DURATION);
    sleep(DURATION);
}

void pickup_forks(int nth_philosopher){
    pthread_mutex_lock(&mutex); 
    state[nth_philosopher] = HUNGRY;
    printf ("Philosopher %d is now HUNGRY and trying to pick up forks.\n", nth_philosopher);
    test(nth_philosopher);
    pthread_mutex_unlock(&mutex);
     
    if (state[nth_philosopher] == EATING){} 
    else {
        printf ("Philosopher %d can’t pick up forks and start waiting.\n", nth_philosopher);
        pthread_cond_wait(cond_var+(nth_philosopher+1) % PHILOSOPHER_NUMBER, &mutex);
    }
}

void return_forks(int nth_philosopher) {
    pthread_mutex_lock(&mutex); 
    state[nth_philosopher] = THINKING;
    printf ("Philosopher %d returns forks and then starts TESTING %d and %d.\n", nth_philosopher, LEFT, RIGHT);
    test(LEFT); 
    test(RIGHT);
    pthread_mutex_unlock(&mutex); 
}

void test(int nth_philosopher){
    if ((state[LEFT] != EATING) && (state[nth_philosopher] == HUNGRY) && (state[RIGHT] != EATING)) {
        state[nth_philosopher] = EATING;
        printf ("Philosopher %d is now EATING.\n", nth_philosopher);
        sleep(DURATION);
        pthread_cond_signal(cond_var + nth_philosopher);
    }
}

void* philosophers(void* n) {
    int nth_philosopher = ((Index*)n)->index;

    // thinking
    thinking(nth_philosopher);

    // becoming hungry and start eating
    pickup_forks(nth_philosopher);

    // return the forks
    return_forks(nth_philosopher);

    pthread_exit(0);
}

