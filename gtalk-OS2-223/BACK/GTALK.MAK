#=============================================================
#
#	GTALK.MAK - Makefile for project c:\prog\os2\19z18b1\gtalk.prj
#		Created on 03/17/94 at 04:56
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +GTALK.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = C:\os2\apps\BCOS2\LIB
INCLUDEPATH = C:\os2\apps\bcos2\include


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 obj\video.obj \
 obj\useredit.obj \
 obj\user.obj \
 obj\terminal.obj \
 obj\task.obj \
 obj\sysop.obj \
 obj\sts.obj \
 obj\string.obj \
 obj\schedule.obj \
 obj\rotator.obj \
 obj\protocol.obj \
 obj\mail.obj \
 obj\link.obj \
 obj\io.obj \
 obj\input.obj \
 obj\init.obj \
 obj\gt.obj \
 obj\glink.obj \
 obj\gfiles.obj \
 obj\function.obj \
 obj\event.obj \
 obj\editor.obj \
 obj\diags.obj \
 obj\command.obj \
 obj\com.obj \
 obj\channel.obj \
 obj\bbs.obj \
 obj\alloc.obj

#=============================================================
#		Explicit Rules
#=============================================================
gtalk.exe: gtalk.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /c /s /Toe /ap /L$(LIBPATH) @&&|
C:\os2\apps\BCOS2\LIB\C02.OBJ+
obj\video.obj+
obj\useredit.obj+
obj\user.obj+
obj\terminal.obj+
obj\task.obj+
obj\sysop.obj+
obj\sts.obj+
obj\string.obj+
obj\schedule.obj+
obj\rotator.obj+
obj\protocol.obj+
obj\mail.obj+
obj\link.obj+
obj\io.obj+
obj\input.obj+
obj\init.obj+
obj\gt.obj+
obj\glink.obj+
obj\gfiles.obj+
obj\function.obj+
obj\event.obj+
obj\editor.obj+
obj\diags.obj+
obj\command.obj+
obj\com.obj+
obj\channel.obj+
obj\bbs.obj+
obj\alloc.obj
gtalk,gtalk
C:\os2\apps\BCOS2\LIB\C2MT.LIB+
C:\os2\apps\BCOS2\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
OBJ\VIDEO.OBJ: gtalk.cfg VIDEO.C 

OBJ\USEREDIT.OBJ: gtalk.cfg USEREDIT.C 

OBJ\USER.OBJ: gtalk.cfg USER.C 

OBJ\TERMINAL.OBJ: gtalk.cfg TERMINAL.C 

OBJ\TASK.OBJ: gtalk.cfg TASK.C 

OBJ\SYSOP.OBJ: gtalk.cfg SYSOP.C 

OBJ\STS.OBJ: gtalk.cfg STS.C 

OBJ\STRING.OBJ: gtalk.cfg STRING.C 

OBJ\SCHEDULE.OBJ: gtalk.cfg SCHEDULE.C 

OBJ\ROTATOR.OBJ: gtalk.cfg ROTATOR.C 

OBJ\PROTOCOL.OBJ: gtalk.cfg PROTOCOL.C 

OBJ\MAIL.OBJ: gtalk.cfg MAIL.C 

OBJ\LINK.OBJ: gtalk.cfg LINK.C 

OBJ\IO.OBJ: gtalk.cfg IO.C 

OBJ\INPUT.OBJ: gtalk.cfg INPUT.C 

OBJ\INIT.OBJ: gtalk.cfg INIT.C 

OBJ\GT.OBJ: gtalk.cfg GT.C 

OBJ\GLINK.OBJ: gtalk.cfg GLINK.C 

OBJ\GFILES.OBJ: gtalk.cfg GFILES.C 

OBJ\FUNCTION.OBJ: gtalk.cfg FUNCTION.C 

OBJ\EVENT.OBJ: gtalk.cfg EVENT.C 

OBJ\EDITOR.OBJ: gtalk.cfg EDITOR.C 

OBJ\DIAGS.OBJ: gtalk.cfg DIAGS.C 

OBJ\COMMAND.OBJ: gtalk.cfg COMMAND.C 

OBJ\COM.OBJ: gtalk.cfg COM.C 

OBJ\CHANNEL.OBJ: gtalk.cfg CHANNEL.C 

OBJ\BBS.OBJ: gtalk.cfg BBS.C 

OBJ\ALLOC.OBJ: gtalk.cfg ALLOC.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
gtalk.cfg: gtalk.mak
  copy &&|
-Oi
-Oz
-Oe
-Oc
-L$(LIBPATH)
-I$(INCLUDEPATH)
-H=c:\prog\os2\19z18b1\gtalk.CSM
-vi-
-sm
-d
-O
-Ot
-y
-v
-C
-K
-w-rch
-w-aus
-w-eff
-w-rvl
-w-par
| gtalk.cfg


