// Program: thomasB.cpp
// Author:  Thomas Basden
// Functions for Shiba Survival

#include <GL/glx.h>
#include <string.h>
#include "fonts.h"
#include "thomasB.h"
#define MAXBUTTONS 5

int nbuttons = 0;
Button button[MAXBUTTONS];

void thomasB (int x, int y, GLuint textid)
{
  // Show picture
  float wid = 120.0f;
  glColor3ub(255, 255, 255);
  glPushMatrix();
  glTranslatef((float)x, (float)y, 0);
  glBindTexture(GL_TEXTURE_2D, textid);
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid/2, -wid/2);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid/2,  wid/2);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid/2,  wid/2);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid/2, -wid/2);
  glEnd();
  glPopMatrix();
   
	// Show name
  Rect r;
  r.bot = y;
  r.left = x + 90;
  r.center = 0;
  ggprint16(&r, 16, 0x00ffff00, "Thomas Basden");
  }

void menu()
{
  //render the menu 
	Rect r;
	nbuttons=0;
	
	//size and position
	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 620;
	button[nbuttons].r.bot = 560;
	button[nbuttons].r.right =
	   button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
	   button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
	   button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
	   button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Play");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.3f;
	button[nbuttons].color[1] = 0.3f;
	button[nbuttons].color[2] = 0.6f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
  nbuttons++;

	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 620;
	button[nbuttons].r.bot = 440;
	button[nbuttons].r.right =
	   button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
	   button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
	   button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
	   button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "How to play");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.3f;
	button[nbuttons].color[1] = 0.3f;
	button[nbuttons].color[2] = 0.6f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 620;
	button[nbuttons].r.bot = 320;
	button[nbuttons].r.right =
	   button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
	   button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
	   button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
	   button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "High Scores");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	
	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 620;
	button[nbuttons].r.bot = 200;
	button[nbuttons].r.right =
	   button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
	   button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
	   button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
	   button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Credits");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.3f;
	button[nbuttons].color[1] = 0.3f;
	button[nbuttons].color[2] = 0.6f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 620;
	button[nbuttons].r.bot = 80;
	button[nbuttons].r.right =
	   button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
	   button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
	   button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
	   button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Quit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.3f;
	button[nbuttons].color[1] = 0.3f;
	button[nbuttons].color[2] = 0.6f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	//draw the buttons
	for (int i=0; i< 5; i++) {
		if (button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
				glVertex2i(button[i].r.left-w,  button[i].r.bot-w);
				glVertex2i(button[i].r.left-w,  button[i].r.top+w);
				glVertex2i(button[i].r.right+w, button[i].r.top+w);
				glVertex2i(button[i].r.right+w, button[i].r.bot-w);
				glVertex2i(button[i].r.left-w,  button[i].r.bot-w);
			glEnd();
			glLineWidth(1);
		}
		
		if (button[i].down) {
			glColor3fv(button[i].dcolor);
		} 
		else {
			glColor3fv(button[i].color);
		}
		
		glBegin(GL_QUADS);
			glVertex2i(button[i].r.left,  button[i].r.bot);
			glVertex2i(button[i].r.left,  button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();

		r.left = button[i].r.centerx;
		r.bot  = button[i].r.centery-8;
		r.center = 1;
		
		if (button[i].down) {
			ggprint16(&r, 0, button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, button[i].text_color, button[i].text);
		}
	}    
}