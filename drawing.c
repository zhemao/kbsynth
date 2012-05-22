#include "drawing.h"

void draw_piano(Display *display, Window window, GC gc, int width, int height){
	long white = WhitePixel(display, DefaultScreen(display));
	long black = BlackPixel(display, DefaultScreen(display));
	int whiteKeyWidth = width / 8;
	int blackKeyWidth = width / 16;
	int blackKeyHeight = height * 0.6;
	int i;

	XSetForeground(display, gc, white);

	XFillRectangle(display, window, gc, 0, 0, width, height);

	XSetForeground(display, gc, black);

	for(i=0; i<8; i++){
		XDrawRectangle(display, window, gc, i * whiteKeyWidth, 0, whiteKeyWidth, height);
	}

	for(i=0; i<2; i++){
		XFillRectangle(display, window, gc, (0.75+i) * whiteKeyWidth, 0, 
											blackKeyWidth, blackKeyHeight);
	}

	for(i=3; i<6; i++){
		XFillRectangle(display, window, gc, (0.75+i) * whiteKeyWidth, 0, 
											blackKeyWidth, blackKeyHeight);
	}

	XFlush(display);
}
