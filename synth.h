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
float frequencies[3][13] = {
	{130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65,
		220.00, 233.08, 246.94, 261.63},
	{261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99,
		392.00, 415.30, 440.00, 466.16, 493.88, 523.25},
	{523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61,
		880.00, 932.33, 987.77, 1046.50}
};
					
int characters[13] = {'a', 'w', 's', 'e','d','f','t','g','y','h','u','j', 'k'};

/* array mapping characters to note frequencies */
float char_to_notes[26];

/* map notes to characters */
void synth_init(int octave);
void play_note(ao_device * dev, waveform wf, 
		int note, float amp, float duration, float offset);


#endif /* __SYNTH_H__ */



