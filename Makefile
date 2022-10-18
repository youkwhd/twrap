EXE = twrap

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

INST = /usr/local/bin

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o, $(SRC))
DEPS = $(wildcard *.h)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(EXE)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(EXE)
	mkdir -p $(INST)
	cp $(EXE) $(INST)
	mkdir -p $(INST)/../man/man1/
	cp twrap.1 $(INST)/../man/man1/

uninstall:
	$(RM) $(INST)/$(EXE)
	$(RM) $(INST)/../man/man1/$(EXE).1

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)

.PHONY: $(EXE) clean install uninstall
