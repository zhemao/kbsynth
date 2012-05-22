#ifndef __KBSYNTH_RECORDER_H__
#define __KBSYNTH_RECORDER_H__

#include <stdio.h>
#include <sys/time.h>

typedef struct note {
	int note;
	int octave;
	float time;
	float duration;
} note_t;

void start_recording(char * filename);
void record_note_start(int note, int octave);
void record_note_stop();
void stop_recording();
int scan_note(FILE * f, note_t * note);
double timeval_diff(struct timeval * then, struct timeval * now);

#endif
