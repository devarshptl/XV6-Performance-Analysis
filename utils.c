#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

void panic(char *msg) {
  printf("%s\n", msg);
  exit(1);
}

long now()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec*1000000 + tv.tv_usec;
}


unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

unsigned int intxorbuf(unsigned int *buffer, int size, unsigned int cur) {
    unsigned int result = cur;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

unsigned char bytexorbuf(unsigned char *buffer, int size, unsigned char cur) {
    unsigned char result = cur;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}