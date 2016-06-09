COMPILE_ROOT=../../
include ../../make.config

LIBS=
DEPLIBS=
OBJS= make-sm.o
EXECUTE= make-sm
SUBDIRS=

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

install: all
	cp -R ${EXECUTE} ${TARGET_DIR}

make-sm: make-sm.o

clean:
	rm -f ${OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

