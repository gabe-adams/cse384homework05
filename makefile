#makefile for main.c/homework5
hw5: main.c
	gcc -o hw5 main.c monitor.c backup.c

clean:
	rm hw5

tar:
	tar -cf hw5.tar main.c makefile monitor.c monitor.h backup.c backup.h
