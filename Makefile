# set compiler options here
CFLAGS=-g -Wall -std=c11 
LDLIBS=-lm

# set the name of the executable file to compile here
program=diseaseMonitor

# list object files needed to compile the program
objects=main.o ./Data_structs/LinkedList/LinkedList.o ./Data_structs/AVL/AVL.o ./Data_structs/Hashtable/Hashtable.o ./Data_structs/Hashtable/RecHash.o ./Data_structs/Binarytree/BTPointer.o ./Data_structs/CompleteBT/CBT.o ./Data_structs/BinaryHeap/BH.o Commands.o

# this makes the program. It's the first rule so it will be executed if you give just 'make'
$(program) : $(objects)
	gcc $(objects) $(LDLIBS) -o $(program) 
	
# list .h dependencies for each .o (dependency on .c is automatically set by make!)
# main.o : Stats.h ./data_structs/LRU.h ./inverted_page_table/IPT.h ./simulation_code/mem_lru.h
# mem_lru.o: ../Stats.h ./mem_lru.h
# IPT.o: IPT.h
# LRU.o: LRU.h

# this allows to quicly remove all generated files with 'make clean'
clean:
	rm -rf $(program) $(objects)

# run the program with 'make run'. Sometimes useful to avoid rewriting long commands.
run: $(program)
