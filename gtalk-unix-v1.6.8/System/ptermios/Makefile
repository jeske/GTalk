COMPILE_ROOT=../../
include ../../make.config

INCLUDES=
LIBS=
OBJS=
EXECUTE=ptermios

all:  	${EXECUTE}

ptermios:	ptermios.c 
	${CC} ${CCFLAGS} -o ptermios ptermios.c -I.. ${INCLUDES}

install: all
	cp -R ${EXECUTE} ${TARGET_DIR}

clean:
	${RM} ${OBJS} ${EXECUTE}


