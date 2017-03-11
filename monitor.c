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

		int wd = inotify_add_watch(fd, fileName, IN_MODIFY | IN_DELETE_SELF | IN_ATTRIB);
		if(wd == -1){
			perror("add_watch: ");
			return EXIT_FAILURE;
		}
	else{
		//watch successful, create original backup copy of file 
		if(createBackup(fileName, backupPath, opt_m, opt_t) == EXIT_FAILURE){
			return EXIT_FAILURE;
		}
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
					//printf("Modify: %#08x\n", event->mask & IN_MODIFY);
					//printf("Attrib: %#08x\n", event->mask & IN_ATTRIB);
					//printf("DeleteSelf: %#08x\n", event->mask & IN_DELETE_SELF);
					/*
					 printf("mask = ");
					if (event->mask & IN_ACCESS)        printf("IN_ACCESS ");
					if (event->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
					if (event->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
					if (event->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
					if (event->mask & IN_CREATE)        printf("IN_CREATE ");
					if (event->mask & IN_DELETE)        printf("IN_DELETE ");
					if (event->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
					if (event->mask & IN_IGNORED)       printf("IN_IGNORED ");
					if (event->mask & IN_ISDIR)         printf("IN_ISDIR ");
					if (event->mask & IN_MODIFY)        printf("IN_MODIFY ");
					if (event->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
					if (event->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
					if (event->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
					if (event->mask & IN_OPEN)          printf("IN_OPEN ");
					if (event->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
					if (event->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
					printf("\n");
    */
					if(((event->mask & IN_MODIFY) != 0) | ((event->mask & IN_ATTRIB) != 0)){
						//This if never gets entered when modifying a file in gedit, works for leafpad editor, but not Abiword or gedit. They must delete and recreate the file?
						printf("%s has been modified\n", fileName);
						//Create new backups everytime file 'fileName' has been modified
						if(createBackup(fileName, backupPath, opt_m, opt_t) == EXIT_FAILURE){
							return EXIT_FAILURE;
						}
					}
					else if((event->mask & IN_DELETE_SELF) != 0){
						//Works if file is deleted (from Trash and directory)
						printf("File has been deleted.\n");
						//printf("Remove this debug:\n");
						//printf("DeleteSelf: %#08x\n", event->mask & IN_DELETE_SELF); //prints value in hex
						//printf("Modify: %#08x\n", event->mask & IN_MODIFY); //prints value in hex
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
