#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task3

# Tool invocations
# Executable "flame2" depends on the files *.o.
task3: task3.o 
	ld -melf_i386 task3.o -o task3
	
task3.o: task3.s
	nasm -f elf task3.s -o task3.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task3
