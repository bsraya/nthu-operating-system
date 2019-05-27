# Homework 4

## Dining Philosopher Problem

There are four important functions in this project:

1. `void thinking (int)`
2. `void pickup_forks (int)`
3. `void return_forks (int)`
4. `void test (int)`
5. `void* philosophers (void*)`

![Image result for dining philosophers problem](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/operating_system_din.png)

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

This function is called after a philosopher is done eating. After the philosopher is done eating, his/her state will be set to `THINKING`. 

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

This is the function that binds four of the functions mentioned above.