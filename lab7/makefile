#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task1 task4

# Tool invocations
# Executable "task1" depends on the files *.o.
task1: task1.o 
	gcc -g -m32 -Wall task1.o -o task1
	
task4: task4.o
	gcc -g -m32 -Wall task4.o -o task4

# Depends on the source and header files
task1.o: task1.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o

task4.o: task4.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task4.c -o task4.o
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1
	rm -f *.o task4
