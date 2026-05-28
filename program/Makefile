CC = gcc
CFLAGS = -Wall -Wextra -lm

all: particle_filter

particle_filter: main.c particle.c particle.h
	$(CC) main.c particle.c -o particle_filter $(CFLAGS)

run: particle_filter
	./particle_filter

visual: run
	python visual.py

clean:
	rm -f particle_filter *.csv *.png
