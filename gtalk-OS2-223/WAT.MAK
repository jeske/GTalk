#
#  Makefile
#

#which memory model and processor to use by default
#PROCESSOR =     # for the 16-bit compilers
PROCESSOR = 386 # for the 32-bit compilers

# name the compiler
CC = wcc$(PROCESSOR)
FC = wfc$(PROCESSOR)
CPP = wpp$(PROCESSOR)
ASM = wasm

# options to use
CFLAGS = -i=..\ -w0 -e25 -sg -zq -otexan -d2 -bm -mf -3r -bt=os2
# CFLAGS = -i=..\ /bm /zq /d2 /mf /3s /sg /bt=os2 -za
CCFLAGS = $(CFLAGS)
CPPFLAGS = $(CFLAGS) 
FFLAGS = -noterm -quiet
AFLAGS =
LNKOPT = option quiet system os2v2 option map debug all option stack=16384
OBJS =  diags.obj console.obj useredit.obj user.obj terminal.obj task.obj sysop.obj os2ser.obj string.obj schedule.obj rotator.obj protocol.obj mail.obj link.obj abuf.obj syslog.obj output.obj input.obj init.obj gt.obj glink.obj gfiles.obj function.obj event.obj editor.obj command.obj com.obj channel.obj bbs.obj alloc.obj dos.obj newuser.obj automenu.obj tcpip.obj feedback.obj except.obj filesec.obj ibmfix.obj
LINKOBJS =  diags.obj,console.obj,useredit.obj,user.obj,terminal.obj,task.obj,sysop.obj,os2ser.obj,string.obj,schedule.obj,rotator.obj,protocol.obj,mail.obj,link.obj,abuf.obj,syslog.obj,output.obj,input.obj,init.obj,gt.obj,glink.obj,gfiles.obj,function.obj,event.obj,editor.obj,command.obj,com.obj,channel.obj,bbs.obj,alloc.obj,dos.obj,newuser.obj,automenu.obj,tcpip.obj,feedback.obj,except.obj,filesec.obj,ibmfix.obj

.EXTENSIONS:
.EXTENSIONS: .exe .rex .lib .obj .wbj .asm .c .for .cpp

.BEFORE
        @set INCLUDE=.;$(%watcom)\h;$(%watcom)\h\os2;$(%watcom)\h\sys
        @set LIB=..\lib;$(%watcom)\lib386\os2;$(%watcom)\lib386;d:\addons\tcpip\lib
#        @set FINCLUDE=$(%watcom)\src\fortran;$(%watcom)\src\fortran\dos
#        @set DOS4G=QUIET

#!ifeq PROCESSOR 386
exes = &
    bbs.exe
#!else
#exes = &
#    fhandles.exe &
#    handles.exe
#!endif


# explicit rules

all :   $(exes) .SYMBOLIC
        @%null

bbs.exe : $(OBJS)  .PRECIOUS
        wlink $(LNKOPT) name bbs library so32dll.lib,tcp32dll.lib file $(LINKOBJS)
	
#mempls40.exp : mempls40.obj .PRECIOUS
#        wlink $(LNKOPT) system pharlap file mempls40 name mempls40

# implicit rules

.c.obj :
        $(CC) $^& $(CFLAGS)

.c.wbj :
        $(CC) $^& $(CFLAGS) -zw -fo=$^&.wbj

.cpp.obj :
        $(CPP) $^& $(CPPFLAGS)

.for.obj :
        $(FC) $^& $(FFLAGS)

.asm.obj :
        $(ASM) $(AFLAGS) $^&


clean: .SYMBOLIC
        del *.obj *.wbj *.exe *.exp *.lib > NULL

