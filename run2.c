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
    blk_cnt = 0;

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

        blk_cnt++; sz += n; bufferPtr+=n;

        if (sz % buff_sz == 0) {
            // reset buffer, calculate xor, check time
            xor = intxorbuf((unsigned int *)buffer, buff_sz/sizeof(int), xor);
            memset(buffer, 0, buff_sz);
            bufferPtr = buffer;
            if (now() - start > 7 * 1000 * 1000)
            {
                break; // break when time is over 7 second
            }
        }
    }

    if (sz % buff_sz != 0) {
        xor = intxorbuf((unsigned int *)buffer, (sz % buff_sz)/sizeof(int) + 1, xor);
    }
    end = now();

    close(fd);
    
    printf("Blocks Read: %ld\n", blk_cnt);
    printf("Bytes Read: %ld\n", sz);
    printf("XOR is %u\n", xor);
    printf("Runtime (microseconds): %ld \n", now() - start);
    printf("Performance: %.2f MB/s\n", (double)sz / (double)(end - start));
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: run2 <filename> <block_size>\n");
        exit(1);
    }

    filename = argv[1];
    blk_sz = atoi(argv[2]);
    start = now();
    readFile();
}