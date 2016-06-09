include ../../make.config

LIBS=
DEPLIBS=
OBJS= useredit.o automenu.o function.o ansi.o output.o main.o user.o \
      input.o newuser.o
EXECUTE= useredit

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

useredit: ${OBJS}


clean:
	rm -f ${OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

