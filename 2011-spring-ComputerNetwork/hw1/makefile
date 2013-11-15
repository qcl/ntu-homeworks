all:SimpleMail

run:SimpleMail
	./SimpleMail cnmail.csie.org 25

gui:gui.c SMCF.o SMCF_GUI.o
	gcc gui.c SMCF.o SMCF_GUI.o -o SimpleMailGui  `pkg-config --cflags --libs gtk+-2.0`

SimpleMail:b97501046.c SMCF.o
	gcc b97501046.c SMCF.o -o SimpleMail

SMCF_GUI.o: SMCF_GUI.c SMCF_GUI.h
	gcc -c SMCF_GUI.c `pkg-config --cflags --libs gtk+-2.0`
	
SMCF.o: SMCF.h SMCF.c
	gcc -c SMCF.c

clean:
	rm -f SimpleMailGui
	rm -f SimpleMail
	rm -f *.o
	
