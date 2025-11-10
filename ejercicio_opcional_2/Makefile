CC = gcc
CFLAGS = -g -Wshadow -Wvla -Wall
LDFLAGS = -lm -pthread

ejercicio2: ejercicio2.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f ejercicio2
