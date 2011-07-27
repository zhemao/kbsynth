/*
 *
 * ao_example.c
 *
 *     Written by Stan Seibert - July 2001
 *
 * Legal Terms:
 *
 *     This source file is released into the public domain.  It is
 *     distributed without any warranty; without even the implied
 *     warranty * of merchantability or fitness for a particular
 *     purpose.
 *
 * Function:
 *
 *     This program opens the default driver and plays a 440 Hz tone for
 *     one second.
 *
 * Compilation command line (for Linux systems):
 *
 *     gcc -lao -ldl -lm -o ao_example ao_example.c
 *
 */

#include <stdio.h>
#include <ao/ao.h>
#include <math.h>
#include <string.h>

#define BUF_SIZE 4096

int main(int argc, char **argv)
{
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	int buf_size;
	short * buffer;
	int sample[2];
	float freq[2], amp[2];
	float duration;
	int i, j;
	
	if(argc < 6){
		printf("Usage: %s amp1 freq1 amp2 freq2 duration\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	for(j=0;j<2;j++){
		amp[j] = atof(argv[1+2*j]);
		freq[j] = atof(argv[2+2*j]);
		printf("Channel %d: amp %f, freq %f\n", j, amp[j], freq[j]);
	}
	
	duration = atof(argv[5]);

	/* -- Initialize -- */

	ao_initialize();

	/* -- Setup for default driver -- */

	default_driver = ao_default_driver_id();
	if(default_driver == -1){
		printf("No default driver\n");
		exit(EXIT_FAILURE);
	}

	memset(&format, 0, sizeof(format));
	format.bits = sizeof(short)*8;
	format.channels = 2;
	format.rate = 44100;
	format.byte_format = AO_FMT_NATIVE;

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}

	/* -- Play some stuff -- */
	buf_size = duration * format.channels * format.rate;
	buffer = (short*)calloc(sizeof(short), buf_size);

	for (i = 0; i < buf_size / 2; i++) {
		for(j=0; j<2; j++){
			buffer[2*i+j] = (short)(amp[j] * 
					sin(2 * M_PI * freq[j] * (float)i / format.rate));
		}
	}
	ao_play(device, (char*)buffer, buf_size*sizeof(short));

	/* -- Close and shutdown -- */
	free(buffer);
	ao_close(device);
	ao_shutdown();
	return (0);
}
