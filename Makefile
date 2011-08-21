OPTS=-ggdb
LDFLAGS=-lm -lao -lncurses

synth: synth.h synth.c
	gcc $(OPTS) synth.c -o synth $(LDFLAGS)

play_tone: play_tone.c
	gcc $(OPTS) play_tone.c -o play_tone $(LDFLAGS)
	
clean:
	rm -f play_tone *.o
