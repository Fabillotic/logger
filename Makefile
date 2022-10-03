OBJ = logger.o example.o
BIN = logger

all: ${BIN}

logger.o: logger.c logger.h
example.o: example.c logger.h

logger: ${OBJ}
	${CC} -o $@ ${OBJ}

run: all
	./${BIN}

clean:
	rm -f ${OBJ} ${BIN}
