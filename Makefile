UMLCC   = umlcc
CC      = gcc-11
CFLAGS  = -Wall -O3

SRC_DIR = src
BLD_DIR = build
OBJ_DIR = $(BLD_DIR)/object
DEP_DIR = $(BLD_DIR)/depend
BIN_DIR = bin

SRC_EXT = .c
INC_EXT = .h
OBJ_EXT = .o
DEP_EXT = .d

MKDIR   = mkdir -p
RM      = rm -rf

SRCS    = $(wildcard $(SRC_DIR)/*$(SRC_EXT)) $(wildcard $(SRC_DIR)/**/*$(SRC_EXT))
OBJS    = $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRCS))
DEPS    = $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(DEP_DIR)/%$(DEP_EXT),$(SRCS))

$(BIN_DIR)/$(UMLCC): $(OBJS)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $^ | sed 's|^\(.*\)\.o:|$(OBJ_DIR)/\1.o:|g' > $@

.PHONY: clean
clean:
	$(RM) $(BIN_DIR) $(BLD_DIR)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif
