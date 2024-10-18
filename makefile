.POSIX:
.SUFFIXES:

SRC=\
    calendar.c\

LD_FLAGS=-lm -I/usr/include/SDL2 -D_REENTRANT -lSDL2
RELEASE_FLAGS=-std=c89 -Os -DNDEBUG -g0 -s -Wall -Wno-unknown-pragmas
DEBUG_FLAGS=-std=c89 -DDEBUG -Wall -Wno-unknown-pragmas -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined

BINNAME=calendar
DESTDIR=
PREFIX=/usr/local

all: $(BINNAME)

$(BINNAME):
	cc $(RELEASE_FLAGS) $(LD_FLAGS) $(SRC) -o $(BINNAME)

debug:
	@ echo "[debug]"
	cc $(DEBUG_FLAGS) $(LD_FLAGS) $(SRC) -o $(BINNAME)

run: $(BINNAME)
	./$(BINNAME) $(ARGS)

clean:
	rm -f $(BINNAME)

install: $(BINNAME)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 $(BINNAME) $(DESTIR)$(PREFIX)/bin/$(BINNAME)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BINNAME)

.PHONY: all $(BINNAME) clean install uninstall
