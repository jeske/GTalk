
include ../make.config

LIBS=
EXTOBJS=../abuf.o
OBJS=extcon.o exttest.o
EXECUTE=exttest
DEPLIBS=

all:  	${EXECUTE} ${DEPLIBS}

exttest:	${EXTOBJS} ${OBJS}
		${CC} ${CCFLAGS} ${LDFLAGS} -o exttest ${OBJS} ${EXTOBJS} ${LIBS}

extcon.o:	extcon.c
		${CC} ${CCFLAGS} ${COMPILEFLAGS} extcon.c -I.. ${DEFINES} ${INCLUDES}

exttest.o:	exttest.c
		${CC} ${CCFLAGS} ${COMPILEFLAGS} exttest.c -I.. ${DEFINES} ${INCLUDES}

clean:
	${RM} ${OBJS} ${EXECUTE} ${DEPLIBS}



