OPTS=-ggdb
LDFLAGS=-lm -lao -lncurses

kbsynth: synth.h synth.c waveforms.o recorder.o
	gcc $(OPTS) synth.c waveforms.o recorder.o -o kbsynth $(LDFLAGS)
	
waveforms.o: waveforms.h waveforms.c
	gcc -c waveforms.c

recorder.o: recorder.h recorder.c
	gcc -c recorder.c

play_tone: play_tone.c
	gcc $(OPTS) play_tone.c -o play_tone $(LDFLAGS)
	
clean:
	rm -f kbsynth play_tone *.o
