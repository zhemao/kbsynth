#include "synth.h"
#include "recorder.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

void synth_init(){
	int i;
	mult = 1.0;
	for(i=0;i<13;i++){
		key_to_note[keys[i]-97] = i;
	}
}

void play_note(ao_device * dev, waveform wf, 
		int note, float amp, float duration, float offset){
	float freq = frequencies[note] * mult;
	int buf_size;
	short * buffer;
	int i,j;
	
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
	fprintf(f, "Usage: %s [-w waveform] [(-r|-p) recordfile]\n\n", name);
	fprintf(f, "Supported waveforms\n");
	fprintf(f, "  sin - pure sine wave\n");
	fprintf(f, "  second - second harmonic\n");
	fprintf(f, "  8bit - eight bit sound\n");
	fprintf(f, "  clip - clipped sine wave\n");
	exit(EXIT_FAILURE);
}

void play_recording(ao_device * device, waveform wf, char * rec_filename){
	FILE * rec_file;
	if(strcmp(rec_filename, "-") == 0)
		rec_file = stdin;
	else rec_file = fopen(rec_filename, "r");
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
			play_note(device, wf, note.note, amp, INPUT_GAP, 0);
			ret = scan_note(rec_file, &note);
		} else {
			usleep(1000);
		}
	}

	fclose(rec_file);
}

void play_keyboard(ao_device * device, waveform wf, 
					int rec, char * rec_filename){
	int ch = 0;
	float amp = SHRT_MAX;
	int octave = 0, note = 0;
	float offset = 0;

	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_window_t window;
	xcb_generic_event_t *event;
	xcb_key_press_event_t *kp;
	xcb_keycode_t keycodes[2] = {0, XCB_NO_SYMBOL};
	xcb_keysym_t key;

	conn = xcb_connect(NULL, NULL);

	if(conn == NULL){
		fprintf(stderr, "Could not open display.\n");
		exit(1);
	}

	screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

	if(screen == NULL){
		fprintf(stderr, "Couldn't grab screen.\n");
		exit(1);
	}
	
	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[2] = {screen->white_pixel, 
							XCB_EVENT_MASK_KEY_PRESS | 
							XCB_EVENT_MASK_KEY_RELEASE};

	window = xcb_generate_id(conn);
	xcb_create_window(conn, 0, window, screen->root, 0, 0, 100, 100, 10,
						XCB_WINDOW_CLASS_INPUT_OUTPUT, 
						screen->root_visual, mask, values);
	xcb_map_window(conn, window);
	xcb_flush(conn);
	
	if(rec) start_recording(rec_filename);
	
	while( 1 ){
		event = xcb_poll_for_event(conn);

		if(event != NULL){
			switch(event->response_type & ~0x80){
				case XCB_KEY_PRESS:
					kp = (xcb_key_press_event_t *) event;
					keycodes[0] = kp->detail;
					key = xcb_key_press_lookup_keysym(
					printf("Key Pressed: %d\n", kp->detail);
					break;
				case XCB_KEY_RELEASE:
					kp = (xcb_key_press_event_t *) event;
					offset = 0;
					ch = 0;
					break;
			}
		}

		if(ch == 0); // don't do anything if ch is 0
		if(ch == 'z'){
			octave--;
			mult /= 2;
		} else if(ch == 'x'){
			octave++;
			mult *= 2;
		} else if(ch == 'c'){
			wf = cycle_waveform();
		} else {
			note = key_to_note[ch-97];
			if(rec) record_note(note, octave);
			play_note(device, wf, note, amp, INPUT_GAP, offset);
			offset += INPUT_GAP;
		}

		free(event);
	}
	
	if(rec) stop_recording();

	xcb_disconnect(conn);
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
