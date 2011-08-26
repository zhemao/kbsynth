#ifndef __WAVEFORM_H__
#define __WAVEFORM_H__

typedef float (*waveform)(float, float, float);

float pure_sine(float, float, float);
float second_harmonic(float, float, float);
float third_harmonic(float, float, float);
float phase_shifted(float, float, float);
float clipped_wave(float, float, float);
waveform string_to_wf(char * str);

#endif /* __WAVEFORM_H__ */

