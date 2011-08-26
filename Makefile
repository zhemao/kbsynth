OPTS=-ggdb
LDFLAGS=-lm -lao -lncurses

kbsynth: synth.h synth.c waveforms.o
	gcc $(OPTS) synth.c waveforms.o -o kbsynth $(LDFLAGS)
	
waveforms.o: waveforms.h waveforms.c
	gcc -c waveforms.c

play_tone: play_tone.c
	gcc $(OPTS) play_tone.c -o play_tone $(LDFLAGS)
	
clean:
	rm -f play_tone *.o
