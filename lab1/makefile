#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task1c task2

# Tool invocations
# Executable "task1c task2" depends on the files task1.o task2.o.
task1c: task1c.o
	gcc -g -m32 -Wall -o task1c task1c.o
	
task2: task2.o
	gcc -g -m32 -Wall -o task2 task2.o

# Depends on the source and header files
task1c.o: task1c.c 
	gcc -m32 -g -Wall -ansi -c -o task1c.o task1c.c 
	
task2.o: task2.c 
	gcc -m32 -g -Wall -ansi -c -o task2.o task2.c 
 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1c
	rm -f *.o task2