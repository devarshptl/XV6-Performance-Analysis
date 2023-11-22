#!/bin/bash
for FSZ in 4 8 16 32 64
do
    for BSZ in 128 512 1024 4096
        do  
            echo "File Size: $FSZ MB, Block Size: $BSZ Byte"
            make BSZ=$BSZ BCNT=$(($FSZ*1000000/$BSZ)) runWrite | tail -n 2
        done
done