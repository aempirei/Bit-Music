CC = gcc
CCFLAGS = -Wall -I. -lm
CFLAGS = -Wall -I.  
TARGETS = bit-music bit.wav bit.mp3

.PHONY: all clean wipe

all: $(TARGETS) $(MODULES) $(SCRIPTS)

bit-music: bit-music.o
	$(CC) $(CCFLAGS) -o $@ $^

bit.wav: bit-music
	./bit-music -w > bit.wav

bit.mp3: bit.wav
	lame -h -b 320 bit.wav bit.mp3

clean:
	rm -f *.o *~

wipe: clean
	rm -f $(TARGETS)
