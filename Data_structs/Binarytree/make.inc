# This file is part of the list module.
#
# Anyone who wants to use the list module should include this file in their
# Makefile, so that compilation is configured properly
#
 
# Set compiler flags. -I... tells the compiler to look in our directory for
# include files, so that #include "BTree.h" works.
#
CFLAGS += -I$(MODULES)/Binarytree

# CONDITIONAL COMPILATION
#
# Use the BT_IMPL=[pointers|arrays] flag to select wheather pointers or arrays
#will be used for the implementation of the binary tree.
#
ifeq ($(BT_IMPL),pointers)
	# pointer implementation
	CFLAGS += -DBT_IMPL_POINTERS
	OBJS += $(MODULES)/Binarytree/BTPointer.o
else
	# array implementation (default)
	CFLAGS += -DBT_IMPL_ARRAYS
	OBJS += $(MODULES)/Binarytree/BTArray.o
endif
