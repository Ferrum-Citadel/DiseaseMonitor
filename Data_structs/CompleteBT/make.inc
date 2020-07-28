 
 
# Set compiler flags. -I... tells the compiler to look in our directory for
#
CFLAGS += -I$(MODULES)/CompleteBT

# We need to compile CBT.o
#
OBJS += $(MODULES)/CompleteBT/CBT.o

# We use the BT module, include its make.inc so that it can configure itself.
#
include $(MODULES)/Binarytree/make.inc
