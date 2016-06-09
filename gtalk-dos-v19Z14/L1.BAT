erase %1.obj
erase %1.exe
erase %1.bin
erase %1.glm
set lib=c:\borlandc\lib
bcc %2 -c -mc! -zCCODE -zDCODE -zRCODE -zECODE -zACODE -zBCODE -zTCODE -zFCODE -zPCODE -zGCODE -zSCODE -zHCODE %1.c > file
bcc %2 -c -mc! -zCCODE -zDCODE -zRCODE -zECODE -zACODE -zBCODE -zTCODE -zFCODE -zPCODE -zGCODE -zSCODE -zHCODE GLMDEF.c >> file
if not errorlevel 1 tlink /C /c /m GL.OBJ+glmdef.obj+%1.obj,%1,,cc.lib
if not errorlevel 1 exe2bin %1.exe
glm %1

