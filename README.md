# congset
LD_PRELOAD library for setting the tcp congestion controller on all sockets via env var

# usage

```
make
TCP_CONGESTION_CONTROLLER=ledbat LD_PRELOAD=congset.so /path/to/your/program
```
