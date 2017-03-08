#include <stdlib.h> //for exit succes 
#include <stdio.h> //for perror
#include <sys/inotify.h> //for notification/ watching files
#include <string.h> //for error string
#include <errno.h> //for error handling
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include "backup.h"
//source: thegeekstuff.com/2010/04/inotify-c-program-example/
#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16) )

int startWatch(bool opt_m, bool opt_t, bool opt_d,
				 char* opt_d_arg, char* fileName)
{
	char buffer[EVENT_BUF_LEN];
	char* p;
	char* backupPath;

	if(opt_d)
		backupPath = opt_d_arg;
	else
		backupPath = "backups";

	int fd = inotify_init();
	if(fd == -1){
		perror("init: ");
		return EXIT_FAILURE;
	}	
	else{

		int wd = inotify_add_watch(fd, fileName, IN_MODIFY | IN_DELETE_SELF);
		if(wd == -1){
			perror("add_watch: ");
			return EXIT_FAILURE;
		}
	else{
		//watch successful, first create backup copy of file 
		createBackup(fileName, backupPath, opt_m, opt_t);
		while(1)
		{
			int x = read(fd, buffer, EVENT_BUF_LEN);
			if(x == -1){
				perror("read: ");
				return	EXIT_FAILURE;
			}
			else{
				
				for( p = buffer; p < buffer + x;){
					struct inotify_event* event = (struct inotify_event*) p;
					if((event->mask & IN_MODIFY) != 0){
						printf("Content has been modified\n");
						createBackup(fileName, backupPath, opt_m, opt_t);
					}
					else if((event->mask & IN_DELETE_SELF) != 0){
						printf("File has been deleted\n");
						//call out to exit the program
						return EXIT_SUCCESS;
					}
					p += sizeof(struct inotify_event) + event->len; 
				}
		}
	}

}
}
	return EXIT_SUCCESS;
}