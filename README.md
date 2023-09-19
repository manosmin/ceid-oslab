
# Operating Systems Lab Project @CEID

This project involves creating C programs to analyze parallel execution scenarios with shared variables, synchronizing processes for efficient heap tree sum calculations, and resolving the readers and writers problem using semaphores to ensure alternating access to a shared database, enhancing system stability.


## Features

- Parallel Execution Analysis with Peterson's Algorithm
- Heap Tree Sum Calculation
- Reader-Writer Problem Solution with Semaphores



## Tech Stack

**Back End**: C


## Deployment

To deploy this project on Linux run

```bash
  gcc -pthread petersen-algorithm.c 
  gcc -pthread -std=c99 readers-writers.c
  gcc -pthread system-calls.c
  ./a.out
```

