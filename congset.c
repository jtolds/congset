#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// gcc -shared -fPIC -o congset.so congset.c -ldl

static int (*real_socket)(int domain, int type, int protocol) = 0;
static int controller_len = 0;
static const char *controller_name = NULL;

void __attribute__((constructor)) init() {
  if (!real_socket) {
    real_socket = dlsym(RTLD_NEXT, "socket");
    if (real_socket) {
      controller_name = secure_getenv("TCP_CONGESTION_CONTROLLER");
      if (controller_name) {
        controller_len = strlen(controller_name);
      }
    }
  }
}

int socket(int domain, int type, int protocol) {
  int fd = 0;
  int errno_tmp = 0;

  if (!real_socket) {
    fprintf(stderr, "Error in dlsym or LD_PRELOAD init\n");
    exit(1);
  }

  fd = real_socket(domain, type, protocol);
  if ((domain != AF_INET && domain != AF_INET6) ||
      !(type & SOCK_STREAM) ||
      (protocol != 0 && protocol != IPPROTO_TCP) ||
      fd < 0 ||
      controller_len == 0) {
    return fd;
  }

  errno_tmp = errno;
  setsockopt(fd, IPPROTO_TCP, TCP_CONGESTION, controller_name, controller_len);
  errno = errno_tmp;
  return fd;
}
