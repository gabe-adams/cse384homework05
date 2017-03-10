#include <stdbool.h>
#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#define MAX_PATH 4096
size_t revNum = 0;

char* createCopyName(char* backupPath, bool opt_t, char* fileName){
	printf("got here\n");
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
		revNum += 1;
		char rev_buf[10];
		sprintf(rev_buf, "_rev%d", revNum);
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
    char* copyName =  createCopyName(backupPath, opt_t, fileName);
    printf("copyName = %s", copyName);
    if ((fp1 = fopen(fileName,"r")) == NULL)    
    {    
        printf("\nFile cannot be opened");
        return EXIT_FAILURE;

    }
    else     
    {
        printf("\nFile opened for copy...\n ");    
    }

    fp2 = fopen(copyName, "w");  
    if(fp2 == NULL){
    	perror("fopen: ");
    	return EXIT_FAILURE;
    }

    fseek(fp1, 0L, SEEK_END); // file pointer at end of file

    pos = ftell(fp1);

    fseek(fp1, 0L, SEEK_SET); // file pointer set at start

    while (pos--)
    {
        ch = fgetc(fp1);  // copying file character by character
        fputc(ch, fp2);
    }    
    fclose(fp1);
    fclose(fp2);   
}
