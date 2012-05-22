#include "drawing.h"

void draw_piano(Display *display, Window window, GC gc, int width, int height){
	long white = WhitePixel(display, DefaultScreen(display));
	long black = BlackPixel(display, DefaultScreen(display));
	float whiteKeyWidth = width / 8;
	float blackKeyWidth = width / 16;
	int blackKeyHeight = height * 0.6;
	int i;

	XSetForeground(display, gc, white);

	XFillRectangle(display, window, gc, 0, 0, width, height);

	XSetForeground(display, gc, black);

	for(i=0; i<8; i++){
		XDrawRectangle(display, window, gc, 
				i * whiteKeyWidth, 0, 
				whiteKeyWidth, height);
	}

	for(i=0; i<6; i++){
		if(i != 2){
			XFillRectangle(display, window, gc, 
					(0.75+i) * whiteKeyWidth, 0, 
					blackKeyWidth, blackKeyHeight);
		}
	}

	XFlush(display);
}

static int black_note(int pos){
	if(pos < 2)
		return 2 * pos + 1;
	return 2 * pos;
}

static int white_note(int pos){
	if(pos < 3)
		return 2 * pos;
	if(pos < 7)
		return 2 * pos - 1;
	return 12;
}

int press_key(int x, int y, int width, int height){
	float whiteKeyWidth = width / 8;
	float blackKeyWidth = width / 16;
	int blackKeyHeight = height * 0.6;
	int i;

	if(y < blackKeyHeight){
		for(i=0; i<6; i++){
			if(i != 2){
				int left = (0.75+i) * whiteKeyWidth;
				int right = left + blackKeyWidth;

				if(x > left && x < right)
					return black_note(i);
			}
		}
	}

	for(i=0; i<8; i++){
		int left = i * whiteKeyWidth;
		int right = (i+1) * whiteKeyWidth;

		if(x > left && x < right)
			return white_note(i);
	}

	return -1;
}
