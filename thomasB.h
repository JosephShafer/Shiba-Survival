// Program: thomasB.h
// Author:  Thomas Basden
// Functions for Shiba Survival

#ifndef THOMASB_H
#define THOMASB_H

typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

#endif
