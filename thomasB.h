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

void menu(GLenum target, GLuint texture, int xres, int yres);
void gameOver(int xres, int yres, char* user, float score, GLenum target, GLuint texture);
void howToPlay(int xres, int yres);
extern int location;

#endif