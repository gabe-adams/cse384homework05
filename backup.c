#include <stdbool.h>
#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#define MAX_PATH 4096
size_t revNum = 0;

char* createCopyName(char* backupPath, bool opt_t, char* fileName){
	printf("got here\n");
	if(opt_t){
		//append ISO instead of rev
		return "notDone";
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
void createBackup(char* fileName, char* backupPath, bool opt_m, bool opt_t)
{
    FILE *fp1, *fp2;
    char ch;
    int pos;
    char* copyName =  createCopyName(backupPath, opt_t, fileName);
    printf("copyName = %s", copyName);
    if ((fp1 = fopen(fileName,"r")) == NULL)    
    {    
        printf("\nFile cannot be opened");
        return;

    }
    else     
    {
        printf("\nFile opened for copy...\n ");    
    }

    fp2 = fopen(copyName, "w");  
    if(fp2 == NULL){
    	printf("yer fucked\n");
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