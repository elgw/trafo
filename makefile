CFLAGS=-Wall -Wextra
CFLAGS+=-pedantic -std=gnu11 -fopenmp
CFLAGS+=-O3 -DNDEBUG

LTRAFO_SRC=src/trafo.c \
src/qsort.c \
src/sortbox.c \
src/trafo_util.c \
src/ftab.c \
src/gini.c \
src/entropy.c

VISIBILITY=-DHAVE___ATTRIBUTE__VISIBILITY_HIDDEN -fvisibility=hidden

libtrafo.so:
	$(CC) --shared -fPIC $(VISIBILITY) $(CFLAGS) \
$(LTRAFO_SRC)  -o libtrafo.so
