#include <math.h>
#include "waveforms.h"
#include <stdlib.h>

static waveform all_waveforms[4] = {pure_sine, second_harmonic, 
									third_harmonic, clipped_wave};
static curwf = 0;

float pure_sine(float freq, float amp, float t){
	float theta = 2 * M_PI * freq * t;
	return amp * sin(theta);
}

float second_harmonic(float freq, float amp, float t){
	float theta1 = 2 * M_PI * freq * t;
	float theta2 = 2 * theta1;

	return (amp/2) * sin(theta1) + (amp/2) * sin(theta2);
}

float third_harmonic(float freq, float amp, float t){
	float theta[3];
	float res = 0;
	int i;

	theta[0] = 2 * M_PI * freq * t;
	theta[1] = 2 * theta[0];
	theta[2] = 2 * theta[1];

	res = (amp/2) * sin(theta[0]);
	res += (amp/4) * sin(theta[1]);
	res += (amp/4) * sin(theta[2]);

	return res;
}

float clipped_wave(float freq, float amp, float t){
	float theta = 2 * M_PI * freq * t;
	float res = amp * sin(theta);
	if(res < 1) return 0;
	return res - 1;
}

waveform string_to_wf(char * str){
	if(strcmp(str, "sin")==0){ curwf = 0; return pure_sine; }
	if(strcmp(str, "second")==0){ curwf = 1; return second_harmonic; }
	if(strcmp(str, "third")==0){ curwf = 2; return third_harmonic; }
	if(strcmp(str, "clip")==0){ curwf = 3; return clipped_wave; }
	
	return NULL;
}

waveform cycle_waveform(){
	curwf = (curwf + 1) % 4;
	return all_waveforms[curwf];
}
