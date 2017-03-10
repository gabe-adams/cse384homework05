#include <stdlib.h> //for exit success
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "monitor.h"
//called when no cmdline options or arguments are given
void printUsage(char* name){
	printf("USAGE: %s (-h)(-m)(-t)(-d arg) FILENAME\n", name);
}

void printHelp(){
		printf("-----------------------------------\n");
		printf("Helpful information for Homework 05\n");
		printf("-----------------------------------\n");
		printf("This program is used to create a backup and track changes of an existing file.\n");
		printf("-----------OPTIONS-----------------\n");
		printf("h: display this information page and exit the program\n");
		printf("m: disable meta-data duplication\n");
		printf("t: append duplication time to file name\n");
		printf("d: customize backup location. accepts an argument that is a path to backup folder\n");
}

int main(int argc, char* argv[]){

	//initialize cmdline options
	bool opt_h = false; bool opt_d = false; bool opt_m = false; bool opt_t = false;
	char* opt_d_arg;
	int opt = 0;

	if(argc == 1){
		printUsage(argv[0]);
		return EXIT_FAILURE;
	}

	while((opt = getopt(argc, argv, "hmtd:")) != -1){
		switch(opt){
			case 'h': opt_h = true; 
				break;
			case 'm': opt_m = true; 
				break;
			case 't': opt_t = true; 
				break;
			case 'd': opt_d = true; 
				opt_d_arg = optarg;
				break;
			default:
			//doesn't hit here. usage taken care of before while loop
			//printUsage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	if(opt_h){
		printHelp();
		return EXIT_FAILURE;
	}

	if(opt_d){
		DIR* dir = opendir(opt_d_arg);
		if (dir)
		{
		    /* Directory exists. */
		    closedir(dir);
		}
		else if (ENOENT == errno)
		{
		    printf("The chosen backup directory does not exit.\n");
		    return EXIT_FAILURE;
		}
	}
	//found in monitor.c
	startWatch(opt_m, opt_t, opt_d, opt_d_arg, argv[optind]);
	
	return EXIT_SUCCESS;
}
