OPTS=-ggdb
LDFLAGS=-lm -lao -lncurses

kbsynth: synth.h synth.c
	gcc $(OPTS) synth.c -o kbsynth $(LDFLAGS)

play_tone: play_tone.c
	gcc $(OPTS) play_tone.c -o play_tone $(LDFLAGS)
	
clean:
	rm -f play_tone *.o
