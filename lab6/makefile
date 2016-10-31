#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task 1 task 2 task3 task4

# Tool invocations
# Executable "task3" depends on the files *.o.
task1: task1.o LineParser.o
	gcc -g -m32 -Wall task3.o LineParser.o -o task1
task2: task2.o LineParser.o
	gcc -g -m32 -Wall task3.o LineParser.o -o task2
task3: task3.o LineParser.o
	gcc -g -m32 -Wall task3.o LineParser.o -o task3
task4: task4.o LineParser.o
	gcc -g -m32 -Wall task3.o LineParser.o -o task4

# Depends on the source and header files
LineParser.o: LineParser.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector LineParser.c -o LineParser.o

task1.o: task1.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o	
task2.o: task2.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2.c -o task2.o
task3.o: task3.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task3.c -o task3.o
task4.o: task4.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task4.c -o task4.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1
	rm -f *.o task2
	rm -f *.o task3
	rm -f *.o task4
