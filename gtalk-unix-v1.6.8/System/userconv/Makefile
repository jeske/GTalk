COMPILE_ROOT=../../
include ../../make.config

LIBS=
DEPLIBS=
OBJS= userconv.o
EXECUTE= userconv
SUBDIRS=

.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

userconv: userconv.o

install: all
	cp -R ${EXECUTE} ${TARGET_DIR}


clean:
	rm -f ${OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

