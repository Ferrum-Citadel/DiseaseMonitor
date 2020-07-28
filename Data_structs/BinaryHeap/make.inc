 
 
# Set compiler flags. -I... tells the compiler to look in our directory for
#
CFLAGS += -I$(MODULES)/BinaryHeap

# We need to compile BH.o
#
OBJS += $(MODULES)/BinaryHeap/BH.o

# We use the CBT module, include its make.inc so that it can configure itself.
#
include $(MODULES)/CompleteBT/make.inc
