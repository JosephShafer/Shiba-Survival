// File: danL.cpp
// Author: Daniel Leinker
// Functions for software engineering 3350

#include <GL/glx.h>
#include "fonts.h"
#include "danL.h"
#include "amberZ.h"
#include "josephS.h"
#include "Image.h"

int xres = 1366;
int yres = 768;
const int shiba_size = 80;
const float power_up_size = 20;
const int powerUpInterval = 100;
bool flyingShiba = false;
int flyingShibaPos[2] = {0,500};
//int flyingShibaPos[2];
GLuint powerUpTextures[3];
vector<PowerUp> power_ups;
Image powerUpImage[4] = {
    Image("./images/bone.png",1,1),
    Image("./images/1up.png",1,1),
    Image("./images/sleepingshiba.png",1,1),
    Image("./images/flyer.png",1,1)
};



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


PowerUp::PowerUp(int powerUpType, float ShibaX, float ShibaY) 
{
    type = powerUpType;

    int away = 20;
    //TODO test to make sure these values are coming out right
    position[0] = ((int)ShibaX + (rand() % (xres - away))) % xres;
    position[1] = ((int)ShibaY + (rand() % (yres - away))) % yres;

    #ifdef DEBUG
    printf("\nPowerUp Constructor Type: %i %f %f",type,position[0],position[1]);
    #endif
}

void PowerUp::drawPowerUp()
{

    //glBindTexture();
    //cout << powerUpTextures[0] << endl;
    /*     glColor3f(0.0f, 0.0f, 1.0f);
         glBindTexture(GL_TEXTURE_2D, powerUpTextures[0]);
         glBegin(GL_POLYGON);
            glVertex2f(-power_up_size, power_up_size);
            glVertex2f(power_up_size, power_up_size);
            glVertex2f(power_up_size, -power_up_size);
            glVertex2f(-power_up_size, -power_up_size);
        glEnd(); */
    //drawSprite(powerUpTextures[0],powerUpImage[0],10,10,)
}

bool PowerUp::collisionCheck(float ShibaX, float ShibaY) 
{
    /* bool collisionX = ShibaX + shiba_size >= position[0] &&
        position[0] + power_up_size >= ShibaX; 
    bool collisionY = ShibaY + shiba_size >= position[1] &&
        position[1] + power_up_size >= ShibaY; 
    return collisionX && collisionY; */
    return ((ShibaX - shiba_size <= position[0] && 
        ShibaX >= position[0] - power_up_size) &&
        (ShibaY - shiba_size <= position[1] && 
        ShibaY + 10 >= position[1] - power_up_size - 5));
}

void PowerUp::activatePowerUp() 
{
    if (type == 0) {
        scoreObject.changeScore(100); //Changed name in my file so changed here -Joey
    } else if (type == 1) {
        numLivesLeft.changeLives(1);
    } else if (type == 2) {
        flyingShiba = true;
        flyingShibaPos[0] = 0;
        flyingShibaPos[1] = 400;
        enemyController.cleanupEnemies();
        scoreObject.changeScore(1000);
    }
}

void destroyAllPowerups() 
{
    power_ups.clear();
}

void powerUpPhysicsCheck(float ShibaX, float ShibaY) 
{
    powerUpTimer(ShibaX, ShibaY);
    powerUpCollision(ShibaX, ShibaY);
    if (flyingShiba) {
        flyingShibaPos[0] += 10;
        if (flyingShibaPos[0] > 1766) {
            flyingShiba = false;
            enemyController.cleanupEnemies();
            flyingShibaPos[0] = 0;
        }
    }
}

void powerUpTimer(float ShibaX, float ShibaY) 
{
    //See if we should spawn some powerups
    if ((rand() % (powerUpInterval * 2)) == 1) {
        spawnPowerUp(1,0,ShibaX,ShibaY);
    }
    if ((rand() % (powerUpInterval * 5)) == 1) {
        spawnPowerUp(1,1,ShibaX,ShibaY);
    }
    if ((rand() % (powerUpInterval * 15)) == 1) {
        spawnPowerUp(1,2,ShibaX,ShibaY);
    }
    #ifdef DEBUG
    //printf("\nPowerUpTimer function %i",mil);
    #endif
}

void spawnPowerUp(int num, int powerUpType, float shibaX, float shibaY) 
{
    #ifdef DEBUG
    printf("\nspawnPowerup function");
    #endif
	for(int i = 0; i < num; i++)
		power_ups.push_back(PowerUp(powerUpType, shibaX, shibaY));
}

void powerUpCollision(float ShibaX,float ShibaY) 
{
    //for (int i = 0; i < power_ups.size(); i++) {
    //for (auto & element : power_ups) {
    //for (vector<PowerUp>::iterator it=power_ups.begin(); it != power_ups.end()){
    // If it collides
    // -Apply powerup
    // -Destroy powerup
    for (auto it=power_ups.begin(); it != power_ups.end();) {
        //printf("\n%f",it->position[0]);
        if (it->collisionCheck(ShibaX,ShibaY)) {
            it->activatePowerUp();
            it = power_ups.erase(it);
        } else {
            ++it;
        }
    }
}

void renderPowerUps() 
{
    Image* test = &powerUpImage[0];
	for(unsigned int i = 0; i < power_ups.size(); i++) {
        //cout << "About to draw sprite" << endl;
        //cout << "Power up texture: " << powerUpTextures[0] << endl;
        //cout << "Power up image:   " << powerUpImage << endl;
        float powerUpX = power_ups[i].position[0];
        float powerUpY = power_ups[i].position[1];
        if (power_ups[i].type == 0) {
            drawSprite(powerUpTextures[power_ups[i].type],
                *test,26,12,powerUpX,powerUpY);
        } else if (power_ups[i].type == 1) {
            drawSprite(powerUpTextures[power_ups[i].type],
                *test,25,25,powerUpX,powerUpY);
        } else if (power_ups[i].type == 2) {
            drawSprite(powerUpTextures[power_ups[i].type],
                *test,40,40,powerUpX,powerUpY);
        }
	    //glPushMatrix();
		//glTranslated(power_ups[i].position[0], power_ups[i].position[1], 0);
		//power_ups[i].drawPowerUp();
	    //glPopMatrix();
	}
    //cout << "Render powerups Flying shiba is: " << flyingShiba << " x: " 
    //cout << flyingShibaPos[0] << " y: " << flyingShibaPos[1] << endl;
    if (flyingShiba) {
        drawSprite(powerUpTextures[3],*test,400,400,
            flyingShibaPos[0],flyingShibaPos[1]);
    }
}

bool isShibaFlying() {
    return flyingShiba;
}

void loadPowerUpImages() {
    /*unsigned char *spriteData;
    glGenTextures(1,&powerUpTextures[0]);
    glBindTexture(GL_TEXTURE_2D, powerUpTextures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //Image* temp = &powerUpImage[0];
    spriteData = buildAlpha(&powerUpImage[0]);
    //spriteData = buildAlpha(*test);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, powerUpImage[0].width, powerUpImage[0].height, 0, GL_RGBA,GL_UNSIGNED_BYTE, spriteData);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, enemyImages[i].width, enemyImages[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    */
}