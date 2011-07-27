OPTS=-ggdb

play_tone: play_tone.c
	gcc $(OPTS) play_tone.c -o play_tone -lm -lao
	
clean:
	rm -f play_tone *.o
