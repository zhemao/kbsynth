#ifndef __KBSYNTH_DRAWING_H__
#define __KBSYNTH_DRAWING_H__

#include <X11/Xlib.h>

void draw_piano(Display *display, Window window, GC gc, int width, int height);

#endif
