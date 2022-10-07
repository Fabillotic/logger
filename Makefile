BIN = liblogger.a
OBJ = logger.o
SRC = logger.c
INC = logger.h
EXOBJ = example.o
EXSRC = example.c

CFLAGS = -ansi -pedantic -Wall -Wextra

all: ${BIN}

example: ${EXOBJ} ${BIN}
	gcc -o $@ ${EXOBJ} -L. -llogger

${OBJ}: ${SRC} ${INC}
	gcc -c ${CFLAGS} ${SRC} -o $@

${EXOBJ}: ${EXSRC} ${INC}
	gcc -c ${CFLAGS} ${EXSRC} -o $@

${BIN}: ${OBJ}
	ar rcs $@ ${OBJ}

clean:
	rm -f ${BIN} example ${OBJ} ${EXOBJ} test.txt

.PHONY: all clean
