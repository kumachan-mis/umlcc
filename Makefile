UMLCC   = umlcc
CC      = gcc-11
CFLAGS  = -Wall -O3

SRC_DIR = src
BLD_DIR = build
OBJ_DIR = $(BLD_DIR)/object
DEP_DIR = $(BLD_DIR)/depend

SRC_EXT = .c
INC_EXT = .h
OBJ_EXT = .o
DEP_EXT = .d

MKDIR   = mkdir -p
RM      = rm -rf

SRCS    = $(wildcard $(SRC_DIR)/*$(SRC_EXT))
OBJS    = $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRCS))
DEPS    = $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(DEP_DIR)/%$(DEP_EXT),$(SRCS))

$(UMLCC): $(OBJS)
	$(CC) $(CFLAGS) -o $(UMLCC) $(OBJS)

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(MKDIR) $(DEP_DIR)
	$(CC) $(CFLAGS) -MP -MM $^ | sed 's|^\(.*\)\.o:|$(OBJ_DIR)/\1.o:|g' > $@

-include $(DEPS)

.PHONY: clean
clean:
	$(RM) $(UMLCC) $(BLD_DIR)
