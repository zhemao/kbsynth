CC=gcc
CFLAGS=-ggdb -Wall
LDFLAGS=-lm -lao -lxcb

kbsynth: synth.h synth.c waveforms.o recorder.o
	$(CC) $(CFLAGS) synth.c waveforms.o recorder.o -o kbsynth $(LDFLAGS)
	
waveforms.o: waveforms.c waveforms.h

recorder.o: recorder.c recorder.h

clean:
	rm -f kbsynth play_tone *.o
