# Data Distance Calculator

Computes euclidean distances of 20000 elements of dimension 5.

### Pure C
```
gcc -lm -O3 ddcalc.c && ./a.out
```

### C + OpenCL
```
gcc -lm -lOpenCL -O3 ddcalcCL.c && ./a.out
```

### Python + OpenCL (PyOpenCL)
```
python ddcalc.py
```

## System Info

##### CPU
Intel(R) Core(TM) i3-5005U CPU @ 2.00GHz

##### MEM
12GB 1600mhz

##### GPU
Intel(R) HD Graphics 5500 BroadWell U-Processor GT2 4GB

##### OS
Manjaro Linux


## Benchmark
| Part                  | Pure C (CPU) | C + OpenCL (GPU) | Python + OpenCL (GPU) |
| -                     | -            | -                | -                     |
| queue and kernel time | 0.0000       | 0.2313           | 0.0816                |
| data build time       | 0.0037       | 0.0036           | 0.0023 (numpy)        |
| ddcalc time           | **2.8880**   | 0.4252           | 0.4292                |
| copy back time        | 0.0000       | 0.5000           | **2.2126**            |
| total                 | 2.8917       | 1.1601           | 2.7257                |
