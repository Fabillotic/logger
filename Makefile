BIN = liblogger.a
OBJ = logger.o
EXOBJ = example.o

all: ${BIN}

example: ${EXOBJ} ${BIN}
	gcc -o $@ ${EXOBJ} -L. -llogger

${OBJ}:
${EXOBJ}:

${BIN}: ${OBJ}
	ar rcs $@ ${OBJ}

clean:
	rm -f ${BIN} example ${OBJ} ${EXOBJ}

.PHONY: all clean
