#!/bin/bash
for FSZ in 4 8 16 32 64 128 256 512 1024 2048
do
    make filesz=$(($FSZ*1000000)) randFile > /dev/null
    for BSZ in 1 4 16 32 64 128 512 1024 4096
        do  
            make clearCache > /dev/null
            A1=`make BSZ=$BSZ BCNT=$(($FSZ*1000000/$BSZ)) runRead | tail -n 2`
            A2=`make BSZ=$BSZ BCNT=$(($FSZ*1000000/$BSZ)) runRead | tail -n 2`
            echo "File Size: $FSZ MB, Block Size: $BSZ Byte"
            echo "  No Cache:"
            echo "      $A1"
            echo "  With Cache:"
            echo "      $A2"
        done
done