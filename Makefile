BIN = liblogger.a
OBJ = logger.o
SRC = logger.c
INC = logger.h
EXOBJ = example.o
EXSRC = example.c

all: ${BIN}

example: ${EXOBJ} ${BIN}
	gcc -o $@ ${EXOBJ} -L. -llogger

${OBJ}: ${SRC} ${INC}
	gcc -c ${SRC} -o $@

${EXOBJ}: ${EXSRC} ${INC}
	gcc -c ${EXSRC} -o $@

${BIN}: ${OBJ}
	ar rcs $@ ${OBJ}

clean:
	rm -f ${BIN} example ${OBJ} ${EXOBJ} test.txt

.PHONY: all clean
