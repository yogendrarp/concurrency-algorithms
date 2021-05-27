# Bakery Algorithm
The implementation has been done with reference to the books
1. A New Solution of Dijkstra's Concurrent Programming Problem by Leslie Lamport, MassachusettsComputer Associates,Inc. Communications of the ACM, August 1974 Volume 17 Number 8
2. C++ Concurrency in Action by Anthony Williams

## Algorithm
Taken from the paper mentioned above.
```
begin integer j;
    L1: choosing[i] := 1;
    number[i] := 1 + maximum(number[l],..., number[N]); 
    choosing[i] := 0;
    for j = 1 step 1 until N do
    begin
        L2: if choosing[j] ≠ 0 then goto L2;
        L3: if number[j] ≠ 0 and (number [j], j) < (number[i],i) then goto L3;
    end;
   critical section; 
   number[i] := O; 
   noncritical section; 
   goto L1;
end

```

## Implementation
The struct BakeryAlgorithm has two methods entry and exit that facilitates the mutual exclusion to the critical section. The algorithm ensures mutual exclusion, bounded waiting and starvation freedom. 

The algorithm like Peterson's lets for the other processes execute its critical section before it would execute. The outer for loop checks every other process contending for the critical section. The first while loop waits if the jth process is selecting its number, and once it does it checks if the number is not equal to 0, and the number is lower than its number. In the scenarios where two process gets the same number due to concurrent execution the check (number [j], j) < (number[i],i) helps to pick the process with lower id value. 
``` 
(a,b) < (c,d) if a < c, or if a = c and b < d.
```
This ensures that only process gets to execute its critical section.

The process i has to wait until all other processes with number[j] <= number[i] completes executing its critical section. So the process i waits for a bounded limit of time to execute the critical section.

Assuming that a process will complete executing its critical section in a bounded time, and if the same thread wants to re-execute its critical section, it must request for a number again. This implies that any other process of which number is less than new number of j will execute its critical section ensuring starvation freedom.

## Alternative Implementation
The array of choosing can be eliminated with making the number array atomic. Also if there are large number of threads the command to get the max value increases with order of n O(n). If the value of currmax can be stored and updated atomically, the overhead might be managed.

