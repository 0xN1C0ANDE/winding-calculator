CSRCS=$(wildcard *.c)
COBJS=$(patsubst %.c,%.o,$(CSRCS))
EXEC=$(patsubst %.c,%, $(word 1, $(CSRCS)))

CFLAGS=-std=c17 -g -Wall -Werror -pedantic
LDFLAGS=-lm

all: build

build: $(EXEC)

ifeq (, $(shell which colordiff))
DIFF=diff
else
DIFF=colordiff
endif

%: %.c
	@echo -e '\e[1;34mCompilation...\e[m'
	$(CC) $(CFLAGS) $? -o $@ $(LDFLAGS)

clean:
	@echo -e '\e[1;34mCleaning...\e[m'
	$(RM) $(EXEC) *.o a.out

.PHONY: test all format build
