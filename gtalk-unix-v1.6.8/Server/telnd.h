/****************************************

   Telnet session handling routines

 ****************************************/

#ifndef _GTALK_TELND_H
#define _GTALK_TELND_H

int bind_server_to(int port);
void close_all_files_except(int fd1, int fd2, int fd3, int fd4);
void accept_connections_on(int fd);

#endif /* _GTALK_TELND_H */
