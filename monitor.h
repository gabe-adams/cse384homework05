#ifndef _MONITORH_
#define _MONITORH_

#include <stdlib.h> //for exit succes 
#include <stdio.h> //for perror
#include <sys/inotify.h> //for notification/ watching files
#include <string.h> //for error string
#include <errno.h> //for error handling
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

//source: thegeekstuff.com/2010/04/inotify-c-program-example/
#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16) )

int startWatch(bool opt_m, bool opt_t, bool opt_d,
				 char* opt_d_arg, char* fileName);
#endif