// File: danL.cpp
// Author: Daniel Leinker
// Functions for software engineering 3350

#include <GL/glx.h>
#include "fonts.h"
#include "danL.h"
#include "amberZ.h"

int xres = 1366;
int yres = 768;
const float size = 10;
const int powerUpInterval = 3000;

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

    int away = 20;
    //TODO test to make sure these values are coming out right
    position[0] = ((int)ShibaX + (rand() % (xres - away))) % xres;
    position[1] = ((int)ShibaY + (rand() % (yres - away))) % yres;

    #ifdef DEBUG
    printf("\nPowerUp Constructor %f %f",position[0],position[1]);
    #endif
}

void PowerUp::drawPowerUp()
{
         glColor3f(0.0f, 0.0f, 1.0f);
         glBegin(GL_POLYGON);
            glVertex2f(-size, size);
            glVertex2f(size, size);
            glVertex2f(size, -size);
            glVertex2f(-size, -size);
        glEnd();
}

void powerUpPhysicsCheck(float ShibaX, float ShibaY) {
    powerUpTimer(ShibaX, ShibaY);
    powerUpCollisionCheck(ShibaX, ShibaY);
}

void powerUpTimer(float ShibaX, float ShibaY) {
    //See if we should spawn some powerups
    if ((rand() % 100) == 1) {
        spawnPowerUp(1,ShibaX,ShibaY);
    }
    #ifdef DEBUG
    //printf("\nPowerUpTimer function %i",mil);
    #endif
}

void spawnPowerUp(int num, float shibaX, float shibaY) {
    #ifdef DEBUG
    printf("\nspawnPowerup function");
    #endif
	for(int i = 0; i < num; i++)
		power_ups.push_back(PowerUp(shibaX, shibaY));
}

void powerUpCollisionCheck(float ShibaX,float ShibaY) {
    

}

void renderPowerUps() {
	for(unsigned int i = 0; i < power_ups.size(); i++) {
	    glPushMatrix();
		glTranslated(power_ups[i].position[0], power_ups[i].position[1], 0);
		power_ups[i].drawPowerUp();
	    glPopMatrix();
	}
}