# Set the name of the ISO and the game
XBE_TITLE = Arbiter
GEN_XISO = $(XBE_TITLE).iso

# Include C source files
SRCS := $(shell find $(CURDIR) -name '*.c')

# Define the NXDK directory and global include path
NXDK_DIR ?= $(CURDIR)/..
NXDK_CFLAGS += -I$(CURDIR)/include

# Include SDL and the NXDK makefile
NXDK_SDL = y
include $(NXDK_DIR)/Makefile