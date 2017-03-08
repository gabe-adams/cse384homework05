#ifndef _BACKUPH_
#define _BACKUPH_
#include <stdio.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <string.h>
#define MAX_PATH 4096
size_t revNum;
char* createCopyName(char* backupPath, bool opt_t, char* fileName);
//source: http://www.sanfoundry.com/c-program-copy-file/
void createBackup(char* fileName, char* backupPath, bool opt_m, bool opt_t);

#endif