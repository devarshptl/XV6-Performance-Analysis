CC := gcc
CFLAG := -Wall
filesz = 4000000000 # 4G
BCNT = 1000000
BSZ = 4

OBJ := utils.o \
       run.o \
	   run2.o \
	   fast.o

EXE := run run2 fast

ARTIFACTS := $(EXE) \
             $(OBJ) \
			 randFile \
			 output.txt

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

randFile: 
	head -c $(filesz) </dev/urandom >$@

test.txt:
	echo "hello, world" >$@

run: $(OBJ)
	$(CC) run.o utils.o -o $@  

run2: $(OBJ)
	$(CC) run2.o utils.o -o $@  

fast: $(OBJ)
	$(CC) fast.o utils.o -o $@

runRead: run 
	./$< randFile -r $(BSZ) $(BCNT)

runRead2: run2
	./$< randFile $(BSZ)

runWrite: run
	rm -f output.txt
	./$< output.txt -w $(BSZ) $(BCNT) 

clean: 
	rm -f $(ARTIFACTS)

rebuild: clean all

all: run run2 fast randFile

clearCache:
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"

.PHONY: clean rebuild clearCache runRead runWrite randFile
