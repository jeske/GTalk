include ../../make.config

LIBS=
DEPLIBS=
OBJS= dos.o
EXECUTE: dos

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

dos: dos.o


clean:
	rm -f ${OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

