COMPILE_ROOT=../
include ../make.config

LIBS=
DEPLIBS=
GTLIB_OBJS= abufcommon.o bufio.o channelcommon.o list.o \
            list.o shared.o states.o states.o str.o str.o \
            usercommon.o misc.o rotator.o
TARGETS=libgtlib.a
SUBDIRS=

.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${TARGETS} ${SUBDIRS}

install: all

libgtlib.a: ${GTLIB_OBJS}
	rm -f libgtlib.a
	${AR} libgtlib.a ${GTLIB_OBJS} 
	cp libgtlib.a ${TARGET_DIR}

clean:
	rm -f ${GTLIB_OBJS} ${DEPLIBS} ${EXECUTE} ${TARGETS} ./-L.. errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

clobber: clean
	rm -f ${TARGETS}
 

