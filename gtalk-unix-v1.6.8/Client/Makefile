COMPILE_ROOT=../
include ../make.config

GTLIBS= ${LIBS} -lgtlib

DEPLIBS=
GTALK_OBJS= accounting.o ansi.o answer.o automenu.o channelcli.o \
            command.o comparse.o ddial.o editor.o function.o \
            gamecon.o gtmain.o input.o log.o newuser.o output.o \
            states_cli.o user.o useredit.o  abufcli.o squelch.o \
            login_cli.o term_cli.o 
EXECUTE=gtclient
SUBDIRS=

.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<


all:  	${EXECUTE} ${SUBDIRS} 

install: all
	cp gtclient ${TARGET_DIR}

gtclient: ${GTALK_OBJS} ${TARGET_DIR}/libgtlib.a
	${CC} ${LDFLAGS} -o gtclient ${GTALK_OBJS} ${GTLIBS}

clean:
	rm -f ${GTALK_OBJS} ${GTCLIENT_OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

clobber: clean
	rm -f gtclient
