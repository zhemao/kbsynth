#include "synth.h"
#include "recorder.h"
#include <curses.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

void synth_init(){
	int i;
	mult = 1.0;
	for(i=0;i<13;i++){
		key_to_note[keys[i]-97] = i;
	}
}

void play_note(ao_device * dev, waveform wf, 
		int note, float amp, float duration){
	float freq = frequencies[note] * mult;
	int buf_size;
	short * buffer;
	int i,j;
	float time_unit;	
	
	buf_size = duration * (CHANNELS * SAMP_RATE);
	buffer = (short*)calloc(sizeof(short), buf_size);
	
	for (i = 0; i < buf_size / CHANNELS; i++){
		float t = (float)i/SAMP_RATE;
		float samp = wf(freq, amp, t);
		for(j=0; j<CHANNELS; j++){
			buffer[CHANNELS*i+j] = (short)samp;
		}
	}
	ao_play(dev, (char*)buffer, buf_size*sizeof(short));
	
	free(buffer);
}

void print_usage(FILE * f, char * name){
	fprintf(f, "Usage: %s [-w waveform] [-r recordfile]\n\n", name);
	fprintf(f, "Supported waveforms\n");
	fprintf(f, "  sin - pure sine wave\n");
	fprintf(f, "  second - second harmonic\n");
	fprintf(f, "  8bit - eight bit sound\n");
	fprintf(f, "  clip - clipped sine wave\n");
	exit(EXIT_FAILURE);
}

void play_recording(ao_device * device, waveform wf, char * rec_filename){
	FILE * rec_file = fopen(rec_filename, "r");
	struct timezone tz;
	struct timeval start;
	struct timeval now;
	double diff;
	float amp = SHRT_MAX;
	note_t note;

	gettimeofday(&start, &tz);
	
	int ret = scan_note(rec_file, &note);
	while(ret != 0 && ret != EOF){
		gettimeofday(&now, &tz);
		diff = timeval_diff(&start, &now);

		if(diff >= note.time){
			mult = pow(2, note.octave);
			play_note(device, wf, note.note, amp, INPUT_GAP);
			ret = scan_note(rec_file, &note);
		} else {
			usleep(1000);
		}
	}
}

void play_keyboard(ao_device * device, waveform wf, 
					int rec, char * rec_filename){
	int ch;
	float amp = SHRT_MAX;
	int octave = 0, note = 0;

	initscr();
	noecho();
	raw();

	if(rec) start_recording(rec_filename);
	
	while( (ch=getchar()) != 'q'){
		if(ch == 'z'){
			octave--;
			mult /= 2;
		} else if(ch == 'x'){
			octave++;
			mult *= 2;
		} else if(ch == 'c'){
			wf = cycle_waveform();
		} else{
			note = key_to_note[ch-97];
			
			if(rec) record_note(note, octave);
			play_note(device, wf, note, amp, INPUT_GAP);
		}
	}
	
	endwin();
	
	if(rec) stop_recording();
}

int main(int argc, char *argv[]){
	char ch;
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	char rec_filename[256];
	int rec = 0, pb = 0;
	waveform wf = pure_sine;

	while((ch = getopt(argc, argv, "w:r:p:h")) != -1){
		if(ch == 'w'){
			wf = string_to_wf(optarg);
			if(!wf) print_usage(stderr, argv[0]);
		} else if(ch == 'h'){
			print_usage(stdout, argv[0]);
		} else if(ch == 'r'){
			rec = 1;
			strncpy(rec_filename, optarg, 256);
		} else if (ch == 'p'){
			pb = 1;
			strncpy(rec_filename, optarg, 256);
		} else print_usage(stderr, argv[0]);
	}

	synth_init();
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
	
	if(pb)
		play_recording(device, wf, rec_filename);
	else
		play_keyboard(device, wf, rec, rec_filename);

	ao_close(device);
	ao_shutdown();
	return 0;
}
