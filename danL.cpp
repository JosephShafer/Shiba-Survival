// File: danL.cpp
// Author: Daniel Leinker
// Functions for software engineering 3350

#include <GL/glx.h>
#include "fonts.h"
#include "danL.h"
#include "amberZ.h"
#include "josephS.h"

int xres = 1366;
int yres = 768;
const int shiba_size = 40;
const float power_up_size = 10;
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
            glVertex2f(-power_up_size, power_up_size);
            glVertex2f(power_up_size, power_up_size);
            glVertex2f(power_up_size, -power_up_size);
            glVertex2f(-power_up_size, -power_up_size);
        glEnd();
}

bool PowerUp::collisionCheck(float ShibaX, float ShibaY) {
    bool collisionX = ShibaX + shiba_size >= position[0] &&
        position[0] + power_up_size >= ShibaX; 
    bool collisionY = ShibaY + shiba_size >= position[1] &&
        position[1] + power_up_size >= ShibaY; 
    return collisionX && collisionY; 
}

void PowerUp::activatePowerUp() {
    scoreObject.changeScore(100); //Changed name in my file so changed here -Joey
}

void powerUpPhysicsCheck(float ShibaX, float ShibaY) {
    powerUpTimer(ShibaX, ShibaY);
    powerUpCollision(ShibaX, ShibaY);
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

void powerUpCollision(float ShibaX,float ShibaY) {
    //for (int i = 0; i < power_ups.size(); i++) {
    //for (auto & element : power_ups) {
    //for (vector<PowerUp>::iterator it=power_ups.begin(); it != power_ups.end()){
    for (auto it=power_ups.begin(); it != power_ups.end();){
        //printf("\n%f",it->position[0]);
        // If it collides
        // -Apply powerup
        // -Destroy powerup
        if (it->collisionCheck(ShibaX,ShibaY)) {
            it->activatePowerUp();
            it = power_ups.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void renderPowerUps() {
	for(unsigned int i = 0; i < power_ups.size(); i++) {
	    glPushMatrix();
		glTranslated(power_ups[i].position[0], power_ups[i].position[1], 0);
		power_ups[i].drawPowerUp();
	    glPopMatrix();
	}
}