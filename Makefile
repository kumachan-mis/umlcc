CC     := gcc
CFLAGS := -std=c99 -pedantic -W -Wall -Werror

BLD_DIR := build
BIN_DIR := bin

SRC_TARGET  := umlcc
SRC_MAIN    := main
SRC_DIR     := src
SRC_DEP_DIR := $(BLD_DIR)/$(SRC_DIR)/depend

TEST_LIBS    := -lcunit
TEST_TARGET  := test_umlcc
TEST_MAIN    := test_main
TEST_DIR     := tests
TEST_DEP_DIR := $(BLD_DIR)/$(TEST_DIR)/depend

COV_RUN       := scripts/coverage-run.sh
COV_CLEAN     := scripts/coverage-clean.sh
E2ETEST_RUN   := scripts/e2etest-run.sh
E2ETEST_CLEAN := scripts/e2etest-clean.sh

SAMPLE_CFLAGS  := -S -O0 -std=c99 -pedantic -fno-asynchronous-unwind-tables
SAMPLE_SRC_DIR := sample
SAMPLE_ASM_DIR := sample-out

SRC_EXT  := .c
TEST_EXT := .c
ASM_EXT  := .s
INC_EXT  := .h
OBJ_EXT  := .o
GCDA_EXT := .gcda
GCNO_EXT := .gcno
DEP_EXT  := .mk

MKDIR := mkdir -p
RM    := rm -rf

ifeq ($(MAKE_ENV), coverage)
CFLAGS       += -O0 -fprofile-arcs -ftest-coverage
TEST_LIBS    += -lgcov
SRC_OBJ_DIR  := $(BLD_DIR)/$(SRC_DIR)/cobject
TEST_OBJ_DIR := $(BLD_DIR)/$(TEST_DIR)/cobject
else
ifeq ($(MAKE_ENV), debug)
CFLAGS       += -O0 -g -fsanitize=address -fno-omit-frame-pointer
SRC_OBJ_DIR  := $(BLD_DIR)/$(SRC_DIR)/gobject
TEST_OBJ_DIR := $(BLD_DIR)/$(TEST_DIR)/gobject
else
CFLAGS       += -O3
SRC_OBJ_DIR  := $(BLD_DIR)/$(SRC_DIR)/object
TEST_OBJ_DIR := $(BLD_DIR)/$(TEST_DIR)/object
endif
endif

SRC_SRCS := $(wildcard $(SRC_DIR)/*$(SRC_EXT)) $(wildcard $(SRC_DIR)/**/*$(SRC_EXT))
SRC_OBJS := $(patsubst $(SRC_DIR)/%$(SRC_EXT), $(SRC_OBJ_DIR)/%$(OBJ_EXT), $(SRC_SRCS))
SRC_DEPS := $(patsubst $(SRC_DIR)/%$(SRC_EXT), $(SRC_DEP_DIR)/%$(DEP_EXT), $(SRC_SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/*$(TEST_EXT)) $(wildcard $(TEST_DIR)/**/*$(TEST_EXT))
TEST_OBJS := $(patsubst $(TEST_DIR)/%$(TEST_EXT), $(TEST_OBJ_DIR)/%$(OBJ_EXT), $(TEST_SRCS))
TEST_DEPS := $(patsubst $(TEST_DIR)/%$(TEST_EXT), $(TEST_DEP_DIR)/%$(DEP_EXT), $(TEST_SRCS))

SAMPLE_SRCS := $(wildcard $(SAMPLE_SRC_DIR)/*$(SRC_EXT))
SAMPLE_ASMS := $(patsubst $(SAMPLE_SRC_DIR)/%$(SRC_EXT), $(SAMPLE_ASM_DIR)/%$(ASM_EXT), $(SAMPLE_SRCS))


.PRECIOUS: $(SRC_DEPS) $(TEST_DEPS)

.PHONY: build build-debug build-clean
build:
	$(MAKE) $(BIN_DIR)/$(SRC_TARGET)

build-debug:
	$(MAKE) $(BIN_DIR)/$(SRC_TARGET) MAKE_ENV=debug

build-clean:
	$(RM) $(BIN_DIR)/$(SRC_TARGET) $(SRC_OBJ_DIR) $(SRC_DEP_DIR)

.PHONY: unittest unittest-debug unittest-cov unittest-clean
unittest:
	$(MAKE) $(BIN_DIR)/$(TEST_TARGET)
	$(BIN_DIR)/$(TEST_TARGET)

unittest-debug:
	$(MAKE) $(BIN_DIR)/$(TEST_TARGET) MAKE_ENV=debug
	$(BIN_DIR)/$(TEST_TARGET)

unittest-cov:
	$(MAKE) $(BIN_DIR)/$(TEST_TARGET) MAKE_ENV=coverage
	$(BIN_DIR)/$(TEST_TARGET)
	$(COV_RUN)

unittest-clean:
	$(RM) $(BIN_DIR)/$(TEST_TARGET) $(SRC_OBJ_DIR) $(SRC_DEP_DIR) $(TEST_OBJ_DIR) $(TEST_DEP_DIR)
	$(TEST_COV_CLEAN)

.PHONY: e2etest e2etest-debug e2etest-clean
e2etest:
	$(MAKE) $(BIN_DIR)/$(SRC_TARGET)
	$(E2ETEST_RUN)

e2etest-debug:
	$(MAKE) $(BIN_DIR)/$(SRC_TARGET) MAKE_ENV=debug
	$(E2ETEST_RUN)

e2etest-clean:
	$(RM) $(BIN_DIR)/$(SRC_TARGET) $(SRC_OBJ_DIR) $(SRC_DEP_DIR)
	$(E2ETEST_CLEAN)

.PHONY: sample sample-clean
sample:
	$(MAKE) $(SAMPLE_ASMS)

sample-clean:
	$(RM) $(SAMPLE_ASM_DIR)

.PHONY: setup format clean
setup:
	cp .pre-commit .git/hooks/pre-commit

format:
	@clang-format -i $(shell find . -name *.h -o -name *.c)

clean:
	$(RM) $(BIN_DIR) $(BLD_DIR) $(SAMPLE_ASM_DIR)
	$(COV_CLEAN)
	$(E2ETEST_CLEAN)

$(BIN_DIR)/$(SRC_TARGET): $(SRC_OBJS)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(SRC_OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(SRC_DEP_DIR)/%$(DEP_EXT)
ifeq ($(MAKE_ENV), coverage)
	@$(RM) $(patsubst $(SRC_OBJ_DIR)/%$(OBJ_EXT), $(SRC_OBJ_DIR)/%$(GCDA_EXT), $@) \
		   $(patsubst $(SRC_OBJ_DIR)/%$(OBJ_EXT), $(SRC_OBJ_DIR)/%$(GCNO_EXT), $@)
endif
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ -MT $(patsubst $(SRC_DIR)/%$(SRC_EXT), $(SRC_OBJ_DIR)/%$(OBJ_EXT), $<)

$(BIN_DIR)/$(TEST_TARGET): $(filter-out $(SRC_OBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT), $(SRC_OBJS)) $(TEST_OBJS)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ $(TEST_LIBS) -o $@

$(TEST_OBJ_DIR)/%$(OBJ_EXT): $(TEST_DIR)/%$(TEST_EXT) $(TEST_DEP_DIR)/%$(DEP_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DEP_DIR)/%$(DEP_EXT): $(TEST_DIR)/%$(TEST_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ -MT $(patsubst $(TEST_DIR)/%$(TEST_EXT), $(TEST_OBJ_DIR)/%$(OBJ_EXT), $<)

$(SAMPLE_ASM_DIR)/%$(ASM_EXT): $(SAMPLE_SRC_DIR)/%$(SRC_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(SAMPLE_CFLAGS) -S $< -o $@

ifeq ($(MAKECMDGOALS), $(BIN_DIR)/$(SRC_TARGET))
-include $(SRC_DEPS)
endif

ifeq ($(MAKECMDGOALS), $(BIN_DIR)/$(TEST_TARGET))
-include $(SRC_DEPS) $(TEST_DEPS)
endif
