#!/bin/bash
make randFile > /dev/null
for BSZ in 1 4 16 32 64 128 512 1024 4096 8192
    do  
        make clearCache > /dev/null
        A1=`make BSZ=$BSZ runRead2 | tail -n 1`
        A2=`make BSZ=$BSZ runRead2 | tail -n 1`
        echo "Block Size: $BSZ Byte"
        echo "  No Cache:"
        echo "      $A1"
        echo "  With Cache:"
        echo "      $A2"
    done
