# Homework 4

## Dining Philosopher Problem

There are four important functions in this project:

1. `void thinking (int)`
2. `void pickup_forks (int)`
3. `void return_forks (int)`
4. `void test (int)`
5. `void* philosophers (void*)`

![Image result for dining philosophers problem](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/operating_system_din.png)

In this project, `mutex` is highly involved in achieving the expected result . There are two functions,  `void pickup_forks (void)` and  `void return_forks (void)`, that utilize `mutex`. There are two fundamental operations that a mutex is required to provide, `lock` and `unlock`.

What is Mutex? It stands for Mutual Exculive, and it protects resources from being used by multiple threads. It only allows one thread in and blocking any other threads from accessing at the same time. If a thread wishes to use the mutex, it must call `pthread_mutex_lock(&mutex);` to occupy the resources and then call `pthread_mutex_unlock(&mutex);` to release the resources so the resources can be used by another thread.



### `void thinking (int)`

```c
void thinking(int nth_philosopher) {
    printf("Philosoper %d is now THINKING for %d seconds.\n", nth_philosopher, DURATION);
    sleep(DURATION);
}
```

This function is responsible for putting all of the philosophers in THINKING state, or we can say SLEEP state, since none of them is doing anything for a certain period of time.



###  `void pickup_forks (int)`

```c
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
```

This function is responsible for turning a certain philosopher's state into `HUNGRY` and make sure he is `EATING` by utilizing `mutex`. After releasing the `lock`, it has to check again whether that philosopher is still `EATING`. If he is, does nothing. If he isn't, it has to wait for the other philosopher done `EATING`.



### `void return_forks(int)`

```c
void return_forks(int nth_philosopher) {
    pthread_mutex_lock(&mutex); 
    state[nth_philosopher] = THINKING;
    printf ("Philosopher %d returns forks and then starts TESTING %d and %d.\n", nth_philosopher, LEFT, RIGHT);
    test(LEFT); 
    test(RIGHT);
    pthread_mutex_unlock(&mutex); 
}
```

This function is called after a philosopher is done eating. After the philosopher is done eating, his/her state will be set to `THINKING`. After setting a certain philosopher's state into `THINKING`, it has to see if the person next to the current philosopher is eligible to eat. 



### `void test (int)`

```c
void test(int nth_philosopher){
    if ((state[LEFT] != EATING) && (state[nth_philosopher] == HUNGRY) && (state[RIGHT] != EATING)) {
        state[nth_philosopher] = EATING;
        printf ("Philosopher %d is now EATING.\n", nth_philosopher);
        sleep(DURATION);
        pthread_cond_signal(cond_var + nth_philosopher);
    }
}
```

This function can be called as `void eating (int)` function, since this is where `eating` process is actually happening. A philosopher is allowed to eat if condition is met when he is hungry and whether the left hand side and right hand side of the philosopher are not eating.



### `void* philosopher (void*)`

```c
void* philosophers(void* n) {
    int nth_philosopher = ((Index*)n)->index;
    thinking(nth_philosopher);
    pickup_forks(nth_philosopher);
    return_forks(nth_philosopher);
    pthread_exit(0);
}
```

This is the function that binds four of the functions mentioned above.

### Result

![](/home/johnbjohn/Pictures/result.png)