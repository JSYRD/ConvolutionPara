KISS, TLDR, Ockham's Razor.


Test Environment: `cat TestEnv.txt`
Simulated Convolution Calculation with:
```c
#define BATCH_SIZE 256      //Equals to loop num.
#define MATRIX_NUM 8        //Simulate CNN trainning.
#define CORE_NUM 16         //Simulate CNN trainning.

#define N 10
#define M 5                 //Convolution Core Size.
```
And try get different running time by changing `N`:
```python
NList = [i for i in range(10, 250, 5)]
...
for i in NList:
        header = open("./matrix.h", "w")
        header.write("#define N %d\n#define M %d\n" % (i, 5))
        header.close()
```
For each `N`, run 10 times and take the average result.