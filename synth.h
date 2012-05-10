#ifndef __SYNTH_H__
#define __SYNTH_H__

#include <ao/ao.h>
#include "waveforms.h"

#define SAMP_RATE 44100
#define CHANNELS 2
#define INPUT_GAP 0.25

/* 
 * the frequencies of notes starting from middle C and going up by half notes 
 * for one octave 
 */ 
float frequencies[13] = {261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 
	369.99, 392.00, 415.30, 440.00, 466.16, 493.88, 523.25 };
					
int keys[13] = {'a', 'w', 's', 'e','d','f','t','g','y','h','u','j', 'k'};

int key_to_note[26];

double mult;

/* map notes to characters */
void synth_init();
void play_note(ao_device * dev, waveform wf, 
		int note, float amp, float duration);

#endif /* __SYNTH_H__ */



