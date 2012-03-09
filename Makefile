all:
	gcc -o aoi -g -Wall aoi.c test.c

test:
	gcc -o testmap -g -Wall testmap.c
