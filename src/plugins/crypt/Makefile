SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%.so,$(SRCS))

all: $(PROGS)

plugins: $(PROGS)

%.so: %.c
	gcc -pedantic -Wall -O2 -fPIC -shared $< -o $@ -ldl

clean:
	rm -rf *.so
