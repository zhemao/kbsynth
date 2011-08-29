#include "synth.h"
#include <curses.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>

void synth_init(int octave){
	int i;
	for(i=0;i<13;i++){
		char_to_notes[characters[i]-97] = frequencies[octave][i];
	}
}

void play_note(ao_device * dev, waveform wf, 
		int note, float amp, float duration, float offset){
	float freq = char_to_notes[note-97];
	int buf_size;
	short * buffer;
	int i,j;
	float time_unit;	
	
	buf_size = duration * (CHANNELS * SAMP_RATE);
	buffer = (short*)calloc(sizeof(short), buf_size);
	
	for (i = 0; i < buf_size / CHANNELS; i++){
		float t = (float)i/SAMP_RATE + offset;
		float samp = wf(freq, amp, t);
		for(j=0; j<CHANNELS; j++){
			buffer[CHANNELS*i+j] = (short)samp;
		}
	}
	ao_play(dev, (char*)buffer, buf_size*sizeof(short));
	
	free(buffer);
}

void print_usage(FILE * f, char * name){
	fprintf(f, "Usage: %s [-w waveform]\n\n", name);
	fprintf(f, "Supported waveforms\n");
	fprintf(f, "  sin - pure sine wave\n");
	fprintf(f, "  second - sine wave w/ second harmonic\n");
	fprintf(f, "  third - sine wave w/ second and third harmonics\n");
	fprintf(f, "  clip - clipped sine wave\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	int ch, lastch;
	float amp = SHRT_MAX;
	int octave = 1;
	float offset = 0;
	waveform wf = pure_sine;

	while((ch = getopt(argc, argv, "w:h")) != -1){
		if(ch == 'w'){
			if(optarg == NULL) print_usage(stderr, argv[0]);
			wf = string_to_wf(optarg);
			if(!wf) print_usage(stderr, argv[0]);
		} else if(ch == 'h'){
			print_usage(stdout, argv[0]);
		} else print_usage(stderr, argv[0]);
	}

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
		} else if(ch == 'c'){
			wf = cycle_waveform();
		} else{
			if(lastch == ch) offset += INPUT_GAP;
			else offset = 0;
			play_note(device, wf, ch, amp, INPUT_GAP, offset);
		}
		lastch = ch;
	}
	
	endwin();

	ao_close(device);
	ao_shutdown();
	return 0;
}
