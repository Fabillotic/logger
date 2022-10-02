OBJ = logger.o main.o
BIN = logger

all: ${BIN}

logger.o: logger.c logger.h
main.o: main.c logger.h

logger: ${OBJ}
	${CC} -o $@ ${OBJ}

run: all
	./${BIN}
