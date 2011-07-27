/* 
 * the frequencies of notes starting from middle C and going up by half notes 
 * for one octave 
 */ 
float frequences[12] = {261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99,
					392.00, 415.30, 440.00, 466.16, 493.88, 523.25};
					
int characters[12] = {'a', 'w', 's', 'e','d','f','t','g','y','h','u','j'};

/* array mapping characters to note frequencies */
float char_to_notes[26];

/* map notes to characters */
void kb_piano_init();
