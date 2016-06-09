COMPILE_ROOT=../../
include ../../make.config

LIBS=
DEPLIBS=
OBJS= userlist.o
EXECUTE= userlist
SUBDIRS=

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

install: all
	cp -R ${EXECUTE} ${TARGET_DIR}

userlist: userlist.o

clean:
	rm -f ${OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

