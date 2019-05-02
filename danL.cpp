// File: danL.cpp
// Author: Daniel Leinker
// Functions for software engineering 3350

#include <GL/glx.h>
#include "fonts.h"
#include "danL.h"

int xres = 1366;
int yres = 768;

void danL(int x, int y, GLuint textid)
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
    ggprint16(&r, 16, 0x00ffff00, "Dan Leinker");
}

vector<PowerUp> power_ups;

PowerUp::PowerUp(float ShibaX, float ShibaY) 
{
    // Type of powerup, Only one type for now
    type = 0;

    position[0] = rand() % xres + 1;
    position[1] = rand() % yres + 1;

    #ifdef DEBUG
    printf("\nPowerUp Constructor");
    #endif
}

void PowerUp::drawPowerUp()
{
        // square until we add sprites
         //sideLength = 10.0f;
         glColor3f(1.0f, -1.0f, -1.0f);
         glBegin(GL_POLYGON);
            glVertex2f(-10.0, 10.0);
            glVertex2f(10.0, 10.0);
            glVertex2f(10.0, -10.0);
            glVertex2f(-10.0, -10.0);
        glEnd();
}

void spawnPowerUp(int num, float shibaX, float shibaY)
{
    printf("\nspawnPowerup function");
	for(int i = 0; i < num; i++)
		power_ups.push_back(PowerUp(shibaX, shibaY));
}

void renderPowerUps()
{
	for(unsigned int i = 0; i < power_ups.size(); i++) {
	    glPushMatrix();
		glTranslated(power_ups[i].position[0], power_ups[i].position[1], 0);
		power_ups[i].drawPowerUp();
	    glPopMatrix();
	}
}