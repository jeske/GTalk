COMPILE_ROOT=./
include make.config

SUBDIRS=Lib Server Client System


.EXPORT_ALL_VARIABLES:

.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<


all:  	${EXECUTE} ${SUBDIRS}

install: all

dummy:

Lib: dummy
	${MAKE_SUB} Lib

Server:	dummy
	${MAKE_SUB} Server

Client: dummy
	${MAKE_SUB} Client

System:	dummy
	${MAKE_SUB} System

ptermios: dummy	
	${MAKE_SUB} ptermios

clean:
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

clobber:
	set -e; for i in ${SUBDIRS}; do make clobber -C $$i; done


