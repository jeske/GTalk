COMPILE_ROOT=../../
include ../../make.config

LIBS=
DEPLIBS=
OBJS= usermk.o classmk.o
EXECUTE= usermk classmk
SUBDIRS=

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

install: all
	cp -R ${EXECUTE} ${TARGET_DIR}

usermk: usermk.o
classmk: classmk.o

clean:
	rm -f ${OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

