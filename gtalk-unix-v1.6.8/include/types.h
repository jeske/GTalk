
/************************************
  
           System Types

*************************************/

#ifndef _GTALK_TYPES_H
#define _GTALK_TYPES_H

typedef unsigned char g_uint8;
typedef char g_int8;
typedef unsigned short g_uint16;
typedef short g_int16;
typedef unsigned int g_uint32;
typedef int g_int32;

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

typedef g_uint32 g_system_t;
typedef int node_id;

#endif   /* _GTALK_TYPES_H */



