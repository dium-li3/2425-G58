# Compiler and flags
CC = gcc
#CFLAGS = -Wall -O0 $(shell pkg-config --cflags glib-2.0) -g
CFLAGS = -Wall -O3 $(shell pkg-config --cflags glib-2.0)
LDFLAGS = $(shell pkg-config --libs glib-2.0)

# Directories
SRC_DIR = src
INC_DIR = include

# Find all subdirectories under src/ and include/
SRC_SUBDIRS = $(shell find $(SRC_DIR) -type d)
INC_SUBDIRS = $(shell find $(INC_DIR) -type d)

# Add -I flags for each subdirectory inside include/
CFLAGS += $(foreach dir, $(INC_SUBDIRS), -I$(dir))

# Find all .c files in src and its subdirectories
SRCS = $(foreach dir, $(SRC_SUBDIRS), $(wildcard $(dir)/*.c))

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
EXE1 = programa-principal

# Default target
all: $(EXE1)

# Link the executable
$(EXE1): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Compile .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(EXE1)
