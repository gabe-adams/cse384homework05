#include <stdbool.h>
#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_PATH 4096
size_t revNum = 0;

char* createCopyName(char* backupPath, bool opt_t, char* fileName){
	if(opt_t){
		//append ISO instead of rev
		time_t rawtime;
		struct tm* curTime;
		char time_buff [25];
		time(&rawtime);
		curTime = localtime(&rawtime);
		strftime(time_buff,25,"%FT%H%M%S", curTime);
		char buffer[MAX_PATH+25];
		strcpy(buffer,backupPath);
		strcat(buffer, "/");
		strcat(buffer, fileName);
		strcat(buffer, time_buff);
		
		char* copyName = buffer;
		
		return copyName;
	}else{
		char rev_buf[10];
		sprintf(rev_buf, "_rev%d", revNum);
		revNum += 1;
		char buffer[MAX_PATH+6];
		strcpy(buffer, backupPath);
		strcat(buffer, "/");
		strcat(buffer, fileName);
		strcat(buffer,rev_buf);
		char* copyName = buffer;
		return copyName;
	}
}
 
//source: http://www.sanfoundry.com/c-program-copy-file/
int createBackup(char* fileName, char* backupPath, bool opt_m, bool opt_t)
{
    FILE *fp1, *fp2;
    char ch;
    int pos;
    int fseekCheck;
    int makeDir;
    struct stat st = {0};
    
    char* copyName =  createCopyName(backupPath, opt_t, fileName);
    printf("backup file = %s\n", copyName);
    
    //Source: stackoverflow.com/questions/7430248/creating-a-new-directory-min-c
	if(stat(backupPath, &st) == -1){
		makeDir = mkdir(backupPath, 0700);
		if(makeDir == -1){
			perror("mkdir: ");
			return EXIT_FAILURE;
		}
	}    
    
    if(opt_m){
		if ((fp1 = fopen(fileName,"r")) == NULL)    
		{    
		    printf("File cannot be opened\n");
		    return EXIT_FAILURE;

		}
		else     
		{
		    //printf("File opened for copy...\n");    
		}
	
		fp2 = fopen(copyName, "w"); 
		
		if(fp2 == NULL){
			perror("fopen: ");
			return EXIT_FAILURE;
		}

		fseekCheck = fseek(fp1, 0L, SEEK_END); // file pointer at end of file
		
		if(fseekCheck != 0){
			printf("fseek failed.");
			return EXIT_FAILURE;
		}

		pos = ftell(fp1);

		fseek(fp1, 0L, SEEK_SET); // file pointer set at start
		
		if(fseekCheck != 0){
			printf("fseek failed.");
			return EXIT_FAILURE;
		}

		while (pos--)
		{
		    ch = fgetc(fp1);  // copying file character by character
		    fputc(ch, fp2);
		}    
		fclose(fp1);
		fclose(fp2); 
	}else{//if not opt_m
		struct stat permCheck;
		int fd3 = stat(fileName, &permCheck);
		if(fd3 == -1){
			perror("stat: ");
			return EXIT_FAILURE;
		}
		int in_fd = open(fileName, O_RDONLY);
		if(in_fd == -1){
			perror("open1: ");
			return EXIT_FAILURE;
		}
		//int perms = permCheck.st_mode & 0000;
		
		int out_fd = open(copyName, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
		if(out_fd == -1){
			perror("open2: ");
			return EXIT_FAILURE;
		}
		char buf[8192];

		while (1) {
			ssize_t result = read(in_fd, &buf[0], sizeof(buf));
			if (!result) break;
			if(result == -1){
				perror("read: ");
				return EXIT_FAILURE;
			}
			int writeStatus = write(out_fd, &buf[0], result);
			if(writeStatus == -1){
				perror("write: ");
				return EXIT_FAILURE;
			}
		}
	
		int perms = chmod(copyName, permCheck.st_mode);
	}  
}
