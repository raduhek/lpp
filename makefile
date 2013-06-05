# Compile tests
CC=gcc
CCFLAGS=-Wall -c

all:
	$(CC) $(CCFLAGS) stack/stack_node.c
	$(CC) $(CCFLAGS) stack/stack.c
	$(CC) $(CCFLAGS) pair/pair.c
	$(CC) $(CCFLAGS) parse_tree/parse_tree.c
	$(CC) $(CCFLAGS) parse_tree/utils.c
	$(CC) $(CCFLAGS) sanitizer/sanitizer.c
	$(CC) $(CCFLAGS) main.c
	$(CC) -o main main.c sanitizer.o parse_tree.o utils.o stack_node.o stack.o pair.o

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o
	rm main 

debug: CCFLAGS += -DDEBUG -g
debug: all

help:
	@echo "Available make options:"
	@echo ""
	@echo "make -> build executable"
	@echo "make debug -> build executable with debug option"
	@echo "make clean -> remove executable and object files"
	@echo "make clean_o -> remove object files"
	@echo "make help -> display this text"
