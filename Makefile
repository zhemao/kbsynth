CC=gcc
CFLAGS=-ggdb -Wall
LDFLAGS=-lm -lao -lX11

OBJS=waveforms.o recorder.o drawing.o

kbsynth: synth.h synth.c $(OBJS)
	$(CC) $(CFLAGS) synth.c $(OBJS) -o kbsynth $(LDFLAGS)
	
waveforms.o: waveforms.c waveforms.h

recorder.o: recorder.c recorder.h

drawing.o: drawing.c drawing.h

clean:
	rm -f kbsynth play_tone *.o
