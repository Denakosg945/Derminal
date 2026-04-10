CFLAGS = -Wall -Wextra -D_DEFAULT_SOURCE -std=c11 -g -O2

all: terminal_bin


terminal_bin: terminal_bin.o input.o commands.o 
	$(CC) $(CFLAGS) terminal_bin.o input.o commands.o -o terminal_bin 

terminal_bin.o: terminal_bin.c   
	$(CC) $(CFLAGS) -c terminal_bin.c  

input.o: input.c 
	$(CC) $(CFLAGS) -c input.c 

commands.o: commands.c 
	$(CC) $(CFLAGS) -c commands.c 

clean: 
	rm -f terminal_bin terminal_bin.o
