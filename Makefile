
CFLAGS = -Wall -g  

CC     = gcc $(CFLAGS)

all : commando

commando.o : commando.c   commando.h

	$(CC) -c commando.c


cmd.o : cmd.c  commando.h

	
	$(CC) -c $<

cmdcol.o : cmdcol.c  commando.h

	$(CC) -c $<


util.o : util.c  commando.h

	$(CC) -c $<


commando : commando.o  cmd.o cmdcol.o util.o  
	$(CC) -o commando  commando.o  cmd.o cmdcol.o util.o
	@echo commando is ready

clean:
	@echo Cleaning up object files
	rm -f *.o commando
include test_Makefile
