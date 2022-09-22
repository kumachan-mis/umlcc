CC     := gcc
CFLAGS := -std=c99 -pedantic -W -Wall -Werror

BLD_DIR := build
BIN_DIR := bin

UMLCC    := umlcc
SRC_MAIN := main
SRC_DIR  := src
DEP_DIR  := $(BLD_DIR)/src/depend

TEST_LIBS    := -lcunit
TEST         := test_umlcc
TEST_MAIN    := test_main
TEST_DIR     := tests
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
GCDA_EXT := .gcda
GCNO_EXT := .gcno
DEP_EXT  := .d

MKDIR := mkdir -p
RM    := rm -rf


ifeq ($(MAKE_ENV),coverage)
CFLAGS       += -O0 -fprofile-arcs -ftest-coverage
TEST_LIBS    += -lgcov
OBJ_DIR      := $(BLD_DIR)/src/cobject
TEST_OBJ_DIR := $(BLD_DIR)/tests/cobject
else
ifeq ($(MAKE_ENV),debug)
CFLAGS       += -O0 -g -fsanitize=address -fno-omit-frame-pointer
OBJ_DIR      := $(BLD_DIR)/src/gobject
TEST_OBJ_DIR := $(BLD_DIR)/tests/gobject
else
CFLAGS       += -O3
OBJ_DIR      := $(BLD_DIR)/src/object
TEST_OBJ_DIR := $(BLD_DIR)/tests/object
endif
endif

SRCS  := $(wildcard $(SRC_DIR)/*$(SRC_EXT)) $(wildcard $(SRC_DIR)/**/*$(SRC_EXT))
OBJS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRCS))
DEPS  := $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(DEP_DIR)/%$(DEP_EXT),$(SRCS))

TESTS     := $(wildcard $(TEST_DIR)/*$(TEST_EXT)) $(wildcard $(TEST_DIR)/**/*$(TEST_EXT))
TEST_OBJS := $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_OBJ_DIR)/%$(OBJ_EXT),$(TESTS))
TEST_DEPS := $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_DEP_DIR)/%$(DEP_EXT),$(TESTS))

SAMPLES     := $(wildcard $(SAMPLE_DIR)/*$(SRC_EXT))
SAMPLE_ASMS := $(patsubst $(SAMPLE_DIR)/%$(SRC_EXT),$(SAMPLE_OUT)/%$(ASM_EXT),$(SAMPLES))

.PRECIOUS: $(DEPS) $(TEST_DEPS)
.PHONY: build build-debug unittest unittest-debug unittest-cov e2etest e2etest-debug clean
.PHONY: sample clean-sample format install-pre-commit

build:
	$(MAKE) $(BIN_DIR)/$(UMLCC)

build-debug:
	$(MAKE) $(BIN_DIR)/$(UMLCC) MAKE_ENV=debug

unittest:
	$(MAKE) $(BIN_DIR)/$(TEST)
	$(BIN_DIR)/$(TEST)

unittest-debug:
	$(MAKE) $(BIN_DIR)/$(TEST) MAKE_ENV=debug
	$(BIN_DIR)/$(TEST)

unittest-cov:
	$(MAKE) $(BIN_DIR)/$(TEST) MAKE_ENV=coverage
	$(BIN_DIR)/$(TEST)
	$(TEST_COV)

e2etest:
	$(MAKE) $(BIN_DIR)/$(UMLCC)
	$(E2E_TEST)

e2etest-debug:
	$(MAKE) $(BIN_DIR)/$(UMLCC) MAKE_ENV=debug
	$(E2E_TEST)

sample:
	$(MAKE) $(SAMPLE_ASMS)

$(BIN_DIR)/$(UMLCC): $(OBJS)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
ifeq ($(MAKE_ENV),coverage)
	@$(RM) $(patsubst $(OBJ_DIR)/%$(OBJ_EXT),$(OBJ_DIR)/%$(GCDA_EXT),$@) \
		   $(patsubst $(OBJ_DIR)/%$(OBJ_EXT),$(OBJ_DIR)/%$(GCNO_EXT),$@)
endif
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ -MT $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$<)


$(BIN_DIR)/$(TEST): $(filter-out $(OBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT),$(OBJS)) $(TEST_OBJS)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ $(TEST_LIBS) -o $@

$(TEST_OBJ_DIR)/%$(OBJ_EXT): $(TEST_DIR)/%$(TEST_EXT) $(TEST_DEP_DIR)/%$(DEP_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DEP_DIR)/%$(DEP_EXT): $(TEST_DIR)/%$(TEST_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ -MT $(patsubst $(TEST_DIR)/%$(TEST_EXT),$(TEST_OBJ_DIR)/%$(OBJ_EXT),$<)

$(SAMPLE_OUT)/%$(ASM_EXT): $(SAMPLE_DIR)/%$(SRC_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(SAMPLE_CFLAGS) -S $< -o $@

clean:
	$(RM) $(BIN_DIR) $(BLD_DIR)
	$(TEST_COV_CLEAN)
	$(E2E_CLEAN)

clean-sample:
	$(RM) $(SAMPLE_OUT)

format:
	@clang-format -i $(shell find . -name *.h -o -name *.c)

install-pre-commit:
	cp .pre-commit .git/hooks/pre-commit

ifeq ($(MAKECMDGOALS),$(BIN_DIR)/$(UMLCC))
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),$(BIN_DIR)/$(TEST))
-include $(DEPS) $(TEST_DEPS)
endif
