CC     := gcc
DB     := lldb
CFLAGS := -std=c99 -pedantic -W -Wall -Werror

BLD_DIR := build
BIN_DIR := bin

UMLCC    := umlcc
SRC_MAIN := main
SRC_DIR  := src
OBJ_DIR  := $(BLD_DIR)/src/object
COBJ_DIR := $(BLD_DIR)/src/cobject
DEP_DIR  := $(BLD_DIR)/src/depend

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
GCDA_EXT := .gcda
GCNO_EXT := .gcno
DEP_EXT  := .d

MKDIR := mkdir -p
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
.PHONY: build build-debug unittest unittest-debug unittest-cov e2etest sample clean clean-sample format install-pre-commit

build:
	$(MAKE) $(BIN_DIR)/$(UMLCC)

build-debug:
	$(MAKE) $(BIN_DIR)/$(UMLCC) DEBUG=true

unittest:
	$(MAKE) $(BIN_DIR)/$(TEST)
	$(BIN_DIR)/$(TEST)

unittest-debug:
	$(MAKE) $(BIN_DIR)/$(TEST) DEBUG=true
	$(DB) $(BIN_DIR)/$(TEST)

unittest-cov:
	$(MAKE) $(BIN_DIR)/$(TEST) COVERAGE=true
	$(BIN_DIR)/$(TEST)
	$(TEST_COV)

e2etest:
	$(MAKE) $(BIN_DIR)/$(UMLCC)
	$(E2E_TEST)

sample:
	$(MAKE) $(SAMPLE_ASMS)

$(BIN_DIR)/$(UMLCC): $(OBJS)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

ifeq ($(COVERAGE),true)
$(COBJ_DIR)/%$(OBJ_EXT): CFLAGS += -O0 -fprofile-arcs -ftest-coverage
$(COBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
	@$(RM) $(patsubst $(COBJ_DIR)/%$(OBJ_EXT),$(COBJ_DIR)/%$(GCDA_EXT),$@) \
		   $(patsubst $(COBJ_DIR)/%$(OBJ_EXT),$(COBJ_DIR)/%$(GCNO_EXT),$@)
else ifeq ($(DEBUG),true)
$(OBJ_DIR)/%$(OBJ_EXT): CFLAGS += -O0 -g
$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
else
$(OBJ_DIR)/%$(OBJ_EXT): CFLAGS += -O3
$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT) $(DEP_DIR)/%$(DEP_EXT)
endif
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%$(DEP_EXT): $(SRC_DIR)/%$(SRC_EXT)
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -MP -MM $< -MF $@ -MT $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$<)

ifeq ($(COVERAGE),true)
$(BIN_DIR)/$(TEST): TEST_LIBS += -lgcov
$(BIN_DIR)/$(TEST): $(filter-out $(COBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT),$(COBJS)) $(TEST_OBJS)
else
$(BIN_DIR)/$(TEST): $(filter-out $(OBJ_DIR)/$(SRC_MAIN)$(OBJ_EXT),$(OBJS)) $(TEST_OBJS)
endif
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $^ $(TEST_LIBS) -o $@

ifeq ($(COVERAGE),true)
$(TEST_OBJ_DIR)/%$(OBJ_EXT): CFLAGS += -O0
else ifeq ($(DEBUG),true)
$(TEST_OBJ_DIR)/%$(OBJ_EXT): CFLAGS += -O0 -g
else
$(TEST_OBJ_DIR)/%$(OBJ_EXT): CFLAGS += -O3
endif
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

ifeq ($(MAKECMDGOALS),)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),build)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),build-debug)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),e2etest)
-include $(DEPS)
endif

ifeq ($(MAKECMDGOALS),unittest)
-include $(DEPS) $(TEST_DEPS)
endif

ifeq ($(MAKECMDGOALS),unittest-debug)
-include $(DEPS) $(TEST_DEPS)
endif

ifeq ($(MAKECMDGOALS),unittest-cov)
-include $(DEPS) $(TEST_DEPS)
endif
