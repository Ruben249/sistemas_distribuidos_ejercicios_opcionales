CC = gcc
CFLAGS = -g -Wshadow -Wvla -Wall
LDFLAGS = -lm -pthread

ejercicio: ejercicio.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f ejercicio
