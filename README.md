# Simple CPU Usage Stats (for Linux)

Prints cpu load every second.

```shell
$ ./cpuusage
cpu    cpu0   cpu1   cpu2   cpu3
1.500  2.000  2.970  1.000  0.990
1.256  1.010  1.010  2.000  1.980
```

## Compile

```
gcc -o cpuusage cpuusage.c -std=gnu99
```
