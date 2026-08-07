CC := gcc

# target file
f ?= test.c
# target dir
d ?= tests
# executable name
o ?= executable

C_FLAGS := -Wshadow -Wall -Wextra
C_SOURCES = $(d)/$(f) $(wildcard src/*.c)
C_OUT := .temp/$(o)
COMPILE = $(CC) $(C_SOURCES) -I include -o $(C_OUT) $(C_FLAGS)

.PHONY: all init compile run
all: init compile run

init:
	@echo "--- simpcl build system ---"
	@mkdir -p .temp
	@echo
	@sleep 0.2

compile:
	@echo "> compiling..."
	$(COMPILE)
	@echo
	@sleep 0.2

run:
	@echo
	@for i in $$(seq 1 1); do \
		sub=$$(expr 2 - $$i); \
		echo -e "\033[2A"; \
		echo -ne "> running in $$sub\r"; \
		echo; \
		sleep 1; \
	done;

	@printf "\n\033[2A> running...   \r\n"
	@sleep 0.4
	@echo
	./$(C_OUT)
	@echo