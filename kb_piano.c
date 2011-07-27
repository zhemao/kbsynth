void kb_piano_init(){
	int i;
	for(i=0;i<12;i++){
		char_to_notes[characters[i]-97] = frequences[i];
	}
}
