# Peterson's Algorithm
The implementation has been done with reference to the books 
1. Operating System Concepts, 10th Edition, Abraham Silberschatz, Greg Gagne, Peter B. Galvin
2. C++ Concurrency in Action by Anthony Williams 

## Algorithm
Taken from the book mentioned above.
```
while (true) { 
    flag[i] = true;
    turn = j;
    while (flag[j] && turn == j)
    ;
        /* critical section */
    flag[i] = false;
         /*remainder section */
}
```

## Non-Atomic implementation

### Observations
The algorithm is theoretically correct abiding to mutual exclusion, progress, bounded waiting and starvation freedom. 
Thread Ti and Tj both set their flag as true and set the turn to the alternative thread. Both execute the command but only persists. If thread Ti executes the turn command in the end then the thread Tj executes its critical section. And Ti will wait in the while loop until flag[j] is set to false by Tj. Hence at any point only one thread will execute its critical section.
Given that both Ti and Tj contend to execute the critical section and assuming Tj is in its critical section then process Ti is in the while loop, and post Tj sets the flag as false Ti will enter the critical section. So one of any two thread contending will execute its critical section first and the other next, ensuring progress. 
Thread Ti in its while loop has to only wait till Tj completes its critical section ensuring bounded wait and post that Ti will execute its critical section making the algorithm free from starvation.

### Problems due to memory ordering
Given that the modern compilers reorder the Load and Store commands, the problem is that the load of flag[otherThreadId] in the while loop might be executed before the other thread sets it to true in its execution block. Which means that both Ti and Tj read the flag of other thread as false, by-passing the loop to execute its critical section.

### Fixes
We can add a memory barrier before the while loop so that the all the LOAD and STORE before the barrier are executed before executing the LOAD and STORE after barrier. In the given code adding 
std::atomic_thread_fence(std::memory_order_seq_cst); at line number 13 in the file /Users/*petersons_algorithm_mem_reordering.cpp*

## Atomic-Implementation
### Observations
The atomic implementation too is theoretically correct abiding to mutual exclusion, progress, bounded waiting and starvation freedom. But given the atomicity don't block the memory reordering, we must add the fence to ensure the memory reordering doesn't produce undesired results.
Two observations that are made are, with the use of atomics and without the fence, the counter value is much close to the expected value of 2000000 as opposed to the first algorithm. Second observation is if the code is reordered like below 
```
int otherThreadId = 1 ^ thread_id; // xor is 1 if thread_id is 0 and 0 if 1
flag[thread_id] = true;
turn = otherThreadId;
// std::atomic_thread_fence(std::memory_order_seq_cst);
while (flag[otherThreadId] && turn == otherThreadId)
    ;
```
instead of 

```
int otherThreadId = 1 ^ thread_id; // xor is 1 if thread_id is 0 and 0 if 1
turn = otherThreadId;
flag[thread_id] = true;
// std::atomic_thread_fence(std::memory_order_seq_cst);
while (flag[otherThreadId] && turn == otherThreadId)
    ;
```

the results is always accurate. I might be missing some essentials of atomics. Will read further to comprehend the above behavior.