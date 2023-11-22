#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "utils.h"

char *file_name;
long block_sz, block_cnt;
long start, end;
enum
{
  READ_MODE,
  WRITE_MODE
} MODE = READ_MODE;

unsigned int readFile()
{
  long i;
  int sz, sz_tot = 0;
  unsigned int xor = 0;
  unsigned int buff_sz = block_sz*1000;
  char *buffer = (char *)malloc(buff_sz);
  char *bufferPtr = buffer;
  memset(buffer, 0, buff_sz);

  // open the file
  start = now();
  int fileFD = open(file_name, O_RDONLY);
  for (i = 0; i < block_cnt; i++)
  {
    sz = read(fileFD, bufferPtr, block_sz);

    if (sz == 0) {
      break;
    }
    
    if (sz == -1)
    {
      perror("read");
      exit(1);
    }

    sz_tot+=sz; bufferPtr+=sz;
    if (sz_tot % buff_sz == 0){
      xor = intxorbuf((unsigned int*)buffer, buff_sz/sizeof(int), xor);
      bufferPtr = buffer;
      memset(buffer, 0, buff_sz);
    }
  }

  if (sz_tot % buff_sz != 0){
    xor = intxorbuf((unsigned int*)buffer, (sz_tot % buff_sz)/sizeof(int) + 1, xor);
  }
  end = now();
  close(fileFD);
  return xor;
}

void writeFile()
{ 
  start = now();
  size_t i;
  int sx;
  char *buffer = (char *)malloc(block_sz);
  int wfd = open(file_name, O_WRONLY | O_CREAT |  O_TRUNC, 0644);
  memset(buffer, 0xff, block_sz);
  for(i =0 ; i < block_cnt; i++) {
    sx = write(wfd, buffer, block_sz);
    if (sx == -1) {
      perror("write");
      exit(1); 
    }
  }
  close(wfd);
  end = now();
}

int main(int argc, char **argv)
{
  int opt;
  long i;

  if (argc < 4 || argc > 5)
  {
    panic("Incorrect number of arguments.\n");
  }

  file_name = argv[1];
  block_sz = atol(argv[argc - 2]);
  block_cnt = atol(argv[argc - 1]);

  while ((opt = getopt(argc, argv, "rw")) != -1)
  {
    switch (opt)
    {
    case 'r':
      MODE = READ_MODE;
      break;
    case 'w':
      MODE = WRITE_MODE;
      break;
    default:
      printf("Unknown: %c, OPTIND is: %d\n", opt, optind);
      break;
    }
  }

  printf("filename: %s\n", file_name);
  printf("block_sz: %ld block_cnt: %ld\n", block_sz, block_cnt);

  switch (MODE)
  {
  case READ_MODE:
    {
      printf("Read\n");
      unsigned int xor = readFile();
      printf("XOR is: %u\n", xor);
      break;
    }
  case WRITE_MODE:
    {
      printf("Write\n");
      writeFile();
      break;
    }
  }

  printf("Runtime (microseconds): %ld\n", end-start);
  printf("Performance is: %.2f MB/s\n", (double) block_cnt*block_sz/(double)(end-start));
  return 0;
}
