COMPILE_ROOT=../
include ../make.config

SUBDIRS=userlist userconv ptermios zeroaccounts

.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INCLUDES) -c -o $*.o $<


all:  	${SUBDIRS}

install: all

dummy:

ptermios: dummy	
	${MAKE_SUB} ptermios

usermk: dummy
	${MAKE_SUB} usermk

userlist: dummy
	${MAKE_SUB} userlist

userconv: dummy
	${MAKE_SUB} userconv

zeroaccounts: dummy
	${MAKE_SUB} zeroaccounts

clean:
	set -e; for i in ${SUBDIRS}; do make clean -C $$i; done
