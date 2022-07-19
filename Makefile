CC         := gcc
CFLAGS     := -O3 -std=c99 -pedantic -W -Wall -Werror

BLD_DIR  := build
BIN_DIR  := bin

UMLCC      := umlcc
SRC_MAIN   := main
SRC_DIR    := src
OBJ_DIR    := $(BLD_DIR)/src/object
COBJ_DIR   := $(BLD_DIR)/src/cobject
DEP_DIR    := $(BLD_DIR)/src/depend

TEST_LIBS    := -lcunit
TEST         := test_umlcc
TEST_MAIN    := test_main
TEST_DIR     := tests
TEST_OBJ_DIR := $(BLD_DIR)/tests/object
TEST_DEP_DIR := $(BLD_DIR)/tests/depend

TEST_COV       := scripts/run-coverage.sh
TEST_COV_CLEAN := scripts/clean-coverage.sh
E2E_TEST       := scripts/run-test.sh
E2E_CLEAN      := scripts/clean-test.sh

SAMPLE_CFLAGS := -S -O0 -std=c99 -pedantic -fno-asynchronous-unwind-tables
SAMPLE_DIR    := sample
SAMPLE_OUT    := sample-out

SRC_EXT  := .c
TEST_EXT := .c
ASM_EXT  := .s
INC_EXT  := .h
OBJ_EXT  := .o
DEP_EXT  := .d

MKDIR := mkdir -p
SH    := bash
RM    := rm -rf

SRCS  := $(wildcard $(SRC_DIR)/*$(SRC_EXT)) $(wildcard $(SRC_DIR)/**/*$(SRC_EXT))
OBJS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRCS))
COBJS := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(COBJ_DIR)/%$(OBJ_EXT),$(SRCS))
DEPS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(DEP_DIR)/%$(DEP_EXT),$(SRCS))

TESTS     := $(wildcard $(TEST_DIR)/*$(TEST_EXT)) $(wildcard $(TEST_DIR)/**/*$(TEST_EXT))
TEST_OBJS := $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_OBJ_DIR)/%$(OBJ_EXT),$(TESTS))
TEST_DEPS := $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_DEP_DIR)/%$(DEP_EXT),$(TESTS))

SAMPLES     := $(wildcard $(SAMPLE_DIR)/*$(SRC_EXT))
SAMPLE_ASMS := $(patsubst $(SAMPLE_DIR)/%$(SRC_EXT),$(SAMPLE_OUT)/%$(ASM_EXT),$(SAMPLES))

.PRECIOUS: $(OBJS) $(DEPS) $(TEST_OBJS) $(TEST_DEPS)
.PHONY: build debug-build unittest unittest-with-coverage e2etest sample format clean clean-sample install-pre-commit

build: $(BIN_DIR)/$(UMLCC)

debug-build: CFLAGS += -g
debug-build: $(BIN_DIR)/$(UMLCC)

unittest:
	$(MAKE) $(BIN_DIR)/$(TEST)
	$(BIN_DIR)/$(TEST)

unittest-with-coverage:
	$(RM) $(BIN_DIR)/$(TEST)
	$(MAKE) $(BIN_DIR)/$(TEST) COVERAGE=true
	$(BIN_DIR)/$(TEST)
	$(SH) $(TEST_COV)

e2etest:
	$(MAKE) $(BIN_DIR)/$(UMLCC)
	$(SH) $(E2E_TEST)

sample: $(SAMPLE_ASMS)

$(BIN_DIR)/$(UMLCC): $(OBJS)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

ifeq ($(COVERAGE),true)
$(COBJ_DIR)/%$(OBJ_EXT): CFLAGS += -fprofile-arcs -ftest-coverage
$(COBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
else
$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
endif
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ \
		-MT $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$<)

ifeq ($(COVERAGE),true)
$(BIN_DIR)/$(TEST): TEST_LIBS += -lgcov
$(BIN_DIR)/$(TEST): $(filter-out $(COBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT),$(COBJS)) $(TEST_OBJS)
else
$(BIN_DIR)/$(TEST): $(filter-out $(OBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT),$(OBJS)) $(TEST_OBJS)
endif
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ $(TEST_LIBS) -o $@

$(TEST_OBJ_DIR)/%$(OBJ_EXT): $(TEST_DIR)/%$(TEST_EXT) $(TEST_DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DEP_DIR)/%$(DEP_EXT): $(TEST_DIR)/%$(TEST_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ \
		-MT $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_OBJ_DIR)/%$(OBJ_EXT),$<)

$(SAMPLE_OUT)/%$(ASM_EXT): $(SAMPLE_DIR)/%$(SRC_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(SAMPLE_CFLAGS) -S $< -o $@

format:
	find . -name *.h -o -name *.c | xargs clang-format -i

clean:
	$(RM) $(BIN_DIR) $(BLD_DIR)
	$(SH) $(TEST_COV_CLEAN)
	$(SH) $(E2E_CLEAN)

clean-sample:
	$(RM) $(SAMPLE_OUT)

install-pre-commit:
	cp .pre-commit .git/hooks/pre-commit

ifeq ($(MAKECMDGOALS),)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),build)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),debug-build)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),e2etest)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),unittest)
-include $(DEPS) $(TEST_DEPS)
endif

ifeq ($(MAKECMDGOALS),unittest-with-coverage)
-include $(DEPS) $(TEST_DEPS)
endif
