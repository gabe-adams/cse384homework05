#include <stdlib.h> //for exit succes
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "monitor.h"
//called when no cmdline options or arguments are given
void printUsage(char* name){
	printf("USAGE: ./%s (-h)(-m)(-t)(-d arg) FILENAME\n", name);
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
			printUsage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	if(opt_h){
		printHelp();
		return EXIT_FAILURE;
	}

	startWatch(opt_m, opt_t, opt_d, opt_d_arg, argv[optind]);

	return EXIT_SUCCESS;
}
