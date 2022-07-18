CC         := gcc
COV        := gcov
CFLAGS     := -O3 -std=c99 -pedantic -W -Wall -Werror

BLD_DIR  := build
COV_DIR  := cov
BIN_DIR  := bin

UMLCC    := umlcc
SRC_MAIN := main
SRC_DIR  := src
OBJ_DIR  := $(BLD_DIR)/src/object
DEP_DIR  := $(BLD_DIR)/src/depend

TEST_LIBS    := -lcunit
TEST         := test_umlcc
TEST_MAIN    := test_main
TEST_DIR     := tests
TEST_OBJ_DIR := $(BLD_DIR)/tests/object
TEST_DEP_DIR := $(BLD_DIR)/tests/depend

E2E_TEST  := scripts/test.sh
E2E_CLEAN := scripts/clean.sh

SAMPLE_CFLAGS := -S -O0 -std=c99 -pedantic -fno-asynchronous-unwind-tables
SAMPLE_DIR    := sample
SAMPLE_OUT    := sample-out

SRC_EXT  := .c
TEST_EXT := .c
ASM_EXT  := .s
INC_EXT  := .h
OBJ_EXT  := .o
DEP_EXT  := .d
COV_EXT  := .cov

MKDIR := mkdir -p
SH    := bash
RM    := rm -rf

SRCS  := $(wildcard $(SRC_DIR)/*$(SRC_EXT)) $(wildcard $(SRC_DIR)/**/*$(SRC_EXT))
OBJS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRCS))
DEPS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(DEP_DIR)/%$(DEP_EXT),$(SRCS))
COVS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(COV_DIR)/%$(COV_EXT),$(filter-out $(SRC_DIR)/$(SRC_MAIN)$(SRC_EXT),$(SRCS)))

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
	$(MAKE) $(BIN_DIR)/$(TEST) COVERAGE=true
	$(BIN_DIR)/$(TEST)
	$(MAKE) $(COVS)

e2etest:
	$(MAKE) $(BIN_DIR)/$(UMLCC)
	$(SH) $(E2E_TEST)

sample: $(SAMPLE_ASMS)

$(BIN_DIR)/$(UMLCC): $(OBJS)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

ifeq ($(COVERAGE),true)
$(OBJ_DIR)/%$(OBJ_EXT): CFLAGS += -fprofile-arcs -ftest-coverage
endif
$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ \
		-MT $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$<)

ifeq ($(COVERAGE),true)
$(BIN_DIR)/$(TEST): TEST_LIBS += -lgcov
endif
$(BIN_DIR)/$(TEST): $(filter-out $(OBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT),$(OBJS)) $(TEST_OBJS)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ $(TEST_LIBS) -o $@

$(TEST_OBJ_DIR)/%$(OBJ_EXT): $(TEST_DIR)/%$(TEST_EXT) $(TEST_DEP_DIR)/%$(DEP_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DEP_DIR)/%$(DEP_EXT): $(TEST_DIR)/%$(TEST_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ \
		-MT $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_OBJ_DIR)/%$(OBJ_EXT),$<)

$(COV_DIR)/%$(COV_EXT): $(OBJ_DIR)/%$(OBJ_EXT)
	$(MKDIR) $(dir $@)
	$(COV) -rt $< >> $@

$(SAMPLE_OUT)/%$(ASM_EXT): $(SAMPLE_DIR)/%$(SRC_EXT)
	$(MKDIR) $(dir $@)
	$(CC) $(SAMPLE_CFLAGS) -S $< -o $@

format:
	find . -name *.h -o -name *.c | xargs clang-format -i

clean:
	$(RM) $(BIN_DIR) $(BLD_DIR) $(COV_DIR)
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
