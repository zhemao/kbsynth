#ifndef __KBSYNTH_DRAWING_H__
#define __KBSYNTH_DRAWING_H__

#include <X11/Xlib.h>

enum {
    WHITE_NOTE,
    BLACK_NOTE
};

void draw_piano(Display *display, Window window, GC gc, int width, int height);
int note_type(int note);
int white_note(int note);
int black_note(int note);

#endif
