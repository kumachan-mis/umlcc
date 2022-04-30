CC      = gcc
CFLAGS  = -O3 -std=c99 -pedantic -Wall -Werror
BLD_DIR = build
BIN_DIR = bin

UMLCC    = umlcc
SRC_MAIN = main
SRC_DIR  = src
OBJ_DIR  = $(BLD_DIR)/src/object
DEP_DIR  = $(BLD_DIR)/src/depend

TEST         = test_umlcc
TEST_MAIN    = test_main
TEST_DIR     = tests
TEST_OBJ_DIR = $(BLD_DIR)/tests/object
TEST_DEP_DIR = $(BLD_DIR)/tests/depend

SRC_EXT  = .c
TEST_EXT = .c
INC_EXT  = .h
OBJ_EXT  = .o
DEP_EXT  = .d

MKDIR = mkdir -p
RM    = rm -rf

SRCS = $(wildcard $(SRC_DIR)/*$(SRC_EXT)) $(wildcard $(SRC_DIR)/**/*$(SRC_EXT))
OBJS = $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRCS))
DEPS = $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(DEP_DIR)/%$(DEP_EXT),$(SRCS))

TESTS     = $(wildcard $(TEST_DIR)/*$(TEST_EXT))
TEST_OBJS = $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_OBJ_DIR)/%$(OBJ_EXT),$(TESTS))
TEST_DEPS = $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_DEP_DIR)/%$(DEP_EXT),$(TESTS))

.PHONY: all test clean format

all: $(BIN_DIR)/$(UMLCC)

test: $(BIN_DIR)/$(TEST)
	$^

$(BIN_DIR)/$(UMLCC): $(OBJS)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $^ | sed 's|^\(.*\)\.o:|$(OBJ_DIR)/\1.o:|g' > $@

$(BIN_DIR)/$(TEST): $(filter-out $(OBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT), $(OBJS)) $(TEST_OBJS)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -lcunit -o $@

$(TEST_OBJ_DIR)/%$(OBJ_EXT): $(TEST_DIR)/%$(TEST_EXT) $(TEST_DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DEP_DIR)/%$(DEP_EXT): $(TEST_DIR)/%$(TEST_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $^ | sed 's|^\(.*\)\.o:|$(OBJ_DIR)/\1.o:|g' > $@

clean:
	$(RM) $(BIN_DIR) $(BLD_DIR)

format:
	find . -name *.h -o -name *.c | xargs clang-format -i

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), format)
-include $(DEPS)
endif
endif

ifeq ($(MAKECMDGOALS), test)
-include $(TEST_DEPS)
endif
