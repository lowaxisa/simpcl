CXX := gcc

# target file
f ?= test.c
# target dir
d ?= tests
# executable name
o ?= executable

COMPILE = $(CXX) $(d)/$(f) -I include/simpcl -o .temp/$(o) -Wshadow -lm

all:
	@mkdir -p .temp

	@echo "compiling..."
	$(COMPILE)
	@echo

	@echo "running..."
	./.temp/$(o)
	@echo