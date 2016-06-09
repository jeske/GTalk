COMPILE_ROOT=../
include ../make.config

LOCAL_LIBS= -lgtlib ${LIBS}

DEPLIBS=
GTALK_OBJS=gt.o fork.o \
		common.o telnd.o srv_abuf.o ddial_serv.o \
		extrn.o \
		srv_channel.o log.o \
 		chuser.o \
		schedule.o \
		rungt.o states_serv.o \
                srv_login.o \
                srv_term.o
EXECUTE=gtalk
SUBDIRS=

.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<

all:  	${EXECUTE} ${SUBDIRS}

install: all
	cp gtalk ${TARGET_DIR}

gtalk:	${GTALK_OBJS} ${TARGET_DIR}/libgtlib.a
	${CC} ${LDFLAGS} -o gtalk ${GTALK_OBJS} ${LOCAL_LIBS}
#	${LD} ${LDFLAGS} -o gtalk ${GTALK_OBJS} ${LOCAL_LIBS}

clean:
	rm -f ${GTALK_OBJS} ${GTCLIENT_OBJS} ${DEPLIBS} ${EXECUTE} errorlog
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done

clobber: clean
	rm -f gtalk

