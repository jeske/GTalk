
        assume  cs:_TEXT,ds:_TEXT,es:NOTHING,ss:NOTHING

        EXTRN   _ginsu_main

        PUBLIC  _jmptl,_old_DS,_old_ES,_arg_addr

CGROUP  group   _TEXT,CODE,_DATA,_CVTSEG,_SCNSEG,_FARDATA,_CONST,_INIT_,_BSS,_BSSEND,_FARBSS

_TEXT   segment para public 'CODE'
        PUSH    BP
        MOV     BP,SP
        MOV     AX,[BP+06]
        MOV     WORD PTR _jmptl,AX
        MOV     AX,[BP+08]
        MOV     WORD PTR _jmptl+2,AX
        MOV     AX,[BP+10]
        MOV     _old_DS,AX
        MOV     AX,[BP+12]
        MOV     _old_ES,AX
        MOV     AX,[BP+14]
        MOV     WORD PTR _arg_addr,AX
        MOV     AX,[BP+16]
        MOV     WORD PTR _arg_addr+2,AX
        POP     BP
        JMP     NEAR PTR _ginsu_main

_jmptl          DD      0
_old_DS         DW      0
_old_ES         DW      0
_arg_addr       DD      0

_TEXT   ends


CODE    segment byte public 'CODE'

CODE    ends

_FARDATA segment para public 'CODE'
_FARDATA ends

_FARBSS segment para public 'CODE'
_FARBSS ends

_DATA   segment para public 'CODE'
_DATA   ends

_CVTSEG segment byte public 'CODE'
_CVTSEG ends

_SCNSEG segment byte public 'CODE'
_SCNSEG ends

_CONST  segment para public 'CODE'
_CONST  ends

_INIT_  segment para public 'CODE'

_INIT_  ends

_INITEND_ segment byte public 'CODE'
_INITEND_ ends

_EXIT_  segment para public 'CODE'
_EXIT_  ends

_EXITEND_ segment byte public 'CODE'
_EXITEND_ ends

_BSS    segment para public 'CODE'
_BSS    ends

_BSSEND segment byte public 'CODE'
_BSSEND ends

        end
