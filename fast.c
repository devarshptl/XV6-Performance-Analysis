#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include "utils.h"

char *filename;
long blk_sz;
long blk_cnt;

long start;
long end;

void readFile()
{
    unsigned int xor = 0;
    unsigned int buff_sz = blk_sz*1000;
    long sz = 0, n;

    char *buffer = (char *)malloc(buff_sz);
    char *bufferPtr = buffer;
    memset(buffer, 0, buff_sz);

    // open the file
    int fd = open(filename, O_RDONLY);
    while ((n = read(fd, bufferPtr, blk_sz))) // break when all content of the file is read
    {
        if (n == -1)
        {
            perror("read");
            exit(1);
        }

        sz += n; bufferPtr+=n;

        if (sz % buff_sz == 0) {
            // reset buffer, calculate xor
            xor = intxorbuf((unsigned int *)buffer, buff_sz/sizeof(int), xor);
            memset(buffer, 0, buff_sz);
            bufferPtr = buffer;
        }
    }

    if (sz % buff_sz != 0) {
        xor = intxorbuf((unsigned int *)buffer, (sz % buff_sz)/sizeof(int) + 1, xor);
    }
    end = now();

    close(fd);
    printf("Blocks Size: %ld\n", blk_sz);
    printf("Bytes Read: %ld\n", sz);
    printf("XOR is %u\n", xor);
    printf("Runtime (microseconds): %ld \n", now() - start);
    printf("Performance: %.2f MB/s\n", (double)sz / (double)(end - start));
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: fast <filename>\n");
        exit(1);
    }

    filename = argv[1];
    blk_sz = 8192;
    start = now();
    readFile();
}