# congset
An LD_PRELOAD library for setting the TCP congestion controller on all sockets of a given process via an environment variable.

# usage

```
make
TCP_CONGESTION_CONTROLLER=ledbat LD_PRELOAD=congset.so /path/to/your/program
```
