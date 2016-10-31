#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task1 task2

# Tool invocations
# Executable "task1" depends on the files *.o.
task1: task1.o 
	gcc -g -m32 -Wall task1.o -o task1
task2: task2.o 
	gcc -g -m32 -Wall task2.o -o task2

# Depends on the source and header files
task1.o: task1.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o
task2.o: task2.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2.c -o task2.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1
	rm -f *.o task2