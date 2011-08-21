#include "synth.h"
#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void synth_init(int octave){
	int i;
	for(i=0;i<13;i++){
		char_to_notes[characters[i]-97] = frequencies[octave][i];
	}
}

void play_note(ao_device * dev, int note, float amp, float duration){
	float freq = char_to_notes[note-97];
	int buf_size;
	short * buffer;
	int i,j;
	float time_unit;	
	
	buf_size = duration * (CHANNELS * SAMP_RATE);
	buffer = (short*)calloc(sizeof(short), buf_size);
	
	for (i = 0; i < buf_size / CHANNELS; i++){
		float t = (float)i/SAMP_RATE;
		float theta = 2 * M_PI * freq * t;
		float samp = (amp * sin(theta));
		for(j=0; j<CHANNELS; j++){
			buffer[CHANNELS*i+j] = (short)samp;
		}
	}
	ao_play(dev, (char*)buffer, buf_size*sizeof(short));
	
	free(buffer);
}

int main(void){
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	int ch, lastch;
	float duration = 0.5;
	float amp = 20000;
	int octave = 1;

	synth_init(octave);
	ao_initialize();

	default_driver = ao_default_driver_id();
	if(default_driver == -1){
		printf("No default driver\n");
		exit(EXIT_FAILURE);
	}

	memset(&format, 0, sizeof(format));
	format.bits = sizeof(short)*8;
	format.channels = CHANNELS;
	format.rate = SAMP_RATE;
	format.byte_format = AO_FMT_NATIVE;

	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}
	
	initscr();
	noecho();
	raw();

	while( (ch=getchar()) != 'q'){
		if(ch == 'z'){
			if(octave > 0){
				octave--;
				synth_init(octave);
			}
		} else if(ch == 'x'){
			if(octave < 2){
				octave++;
				synth_init(octave);
			}
		} else play_note(device, ch, amp, 0.25);
	}
	
	endwin();

	ao_close(device);
	ao_shutdown();
	return 0;
}
