#include "recorder.h"
#include <stdio.h>
#include <sys/time.h>

static struct timeval rec_start;
static struct timezone tz;
static FILE * rec_file;

void start_recording(char * filename){
	rec_file = fopen(filename, "w");
	gettimeofday(&rec_start, &tz);
}

static double timeval_diff(struct timeval * then, struct timeval * now){
	int secs = now->tv_sec - then->tv_sec;
	int usecs = now->tv_usec - then->tv_usec;

	if(usecs < 0){
		secs -= 1;
		usecs += 1000000;
	}

	return secs + (double) usecs / 1000000.0;
}

void record_note(char note, int octave){
	struct timeval now;
	double diff;
	
	gettimeofday(&now, &tz);
	diff = timeval_diff(&rec_start, &now);
	
	fprintf(rec_file, "%c\t%d\t%f\n", note, octave, diff);
}

void stop_recording(){
	fclose(rec_file);
	rec_start.tv_sec = 0;
	rec_start.tv_usec = 0;
}


