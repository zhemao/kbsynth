#include "recorder.h"

static struct timeval rec_start;
static struct timezone tz;
static FILE * rec_file;
static note_t cur_note;

void start_recording(char * filename){
	rec_file = fopen(filename, "w");
	gettimeofday(&rec_start, &tz);
}

double timeval_diff(struct timeval * then, struct timeval * now){
	int secs = now->tv_sec - then->tv_sec;
	int usecs = now->tv_usec - then->tv_usec;

	if(usecs < 0){
		secs -= 1;
		usecs += 1000000;
	}

	return secs + (double) usecs / 1000000.0;
}

void record_note_start(int note, int octave){
	struct timeval now;
	double diff;
	
	gettimeofday(&now, &tz);
	diff = timeval_diff(&rec_start, &now);

	cur_note.note = note;
	cur_note.octave = octave;
	cur_note.time = diff;
}

void record_note_stop(){
	struct timeval now;
	double diff;

	gettimeofday(&now, &tz);
	diff = timeval_diff(&rec_start, &now);

	fprintf(rec_file, "%d\t%d\t%f\t%f\n", 
			cur_note.note, cur_note.octave, 
			cur_note.time, diff - cur_note.time);
}

void stop_recording(){
	fclose(rec_file);
	rec_start.tv_sec = 0;
	rec_start.tv_usec = 0;
}

int scan_note(FILE * f, note_t * note){
	return fscanf(f, "%d\t%d\t%f\t%f\n", 
			&note->note, &note->octave, 
			&note->time, &note->duration);
}
