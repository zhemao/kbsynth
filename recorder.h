#ifndef __KBSYNTH_RECORDER_H__
#define __KBSYNTH_RECORDER_H__

void start_recording(char * filename);
void record_note(char note, int octave);
void stop_recording();

#endif