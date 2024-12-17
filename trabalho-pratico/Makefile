# Compiler and flags
CC = gcc
#CFLAGS = -Wall -Werror=discarded-qualifiers -O0 $(shell pkg-config --cflags glib-2.0) -g
CFLAGS = -Wall -Werror=discarded-qualifiers -O3 $(shell pkg-config --cflags glib-2.0)
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
SRCS_PRINCIPAL = $(filter-out src/mains/main_test.c, $(SRCS))
SRCS_TEST = $(filter-out src/mains/main.c, $(SRCS))

# Object files
OBJS = $(SRCS_PRINCIPAL:.c=.o)
OBJS_TEST = $(SRCS_TEST:.c=.o)

# Recommender object file
OBJ_REC = $(shell find -name "recomendador-linux-x86_64.o")

# Executables
EXE1 = programa-principal
EXE2 = programa-testes

# Default target
all: $(EXE1) $(EXE2)

# Link the executables
$(EXE1): $(OBJS) $(OBJ_REC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(EXE2): $(OBJS_TEST) $(OBJ_REC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Compile .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(EXE1) $(OBJS_TEST) $(EXE2)

clean_res:
	rm -f resultados/*

# Search for complex functions
complexity:
	@echo Only printing when Modified McCabe Cyclomatic Complexity is above 5
	@echo | pmccabe -v
	@pmccabe $(SRCS)| sort -nr | awk '{if($$1>5)print}'