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
const int powerUpInterval = 3000;
GLuint powerUpTextures[3];
Image powerUpImage[3] = {
    Image("./images/bone.png",1,1),
    Image("./images/bone.png",1,1),
    Image("./images/bone.png",1,1)
};
Image* test = &powerUpImage[0];


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
    if ((rand() % 200) == 1) {
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
        //cout << "About to draw sprite" << endl;
        //cout << "Power up texture: " << powerUpTextures[0] << endl;
        //cout << "Power up image:   " << powerUpImage << endl;
        drawSprite(powerUpTextures[0],*test,26,12,power_ups[i].position[0],power_ups[i].position[1]);
	    //glPushMatrix();
		//glTranslated(power_ups[i].position[0], power_ups[i].position[1], 0);
		//power_ups[i].drawPowerUp();
	    //glPopMatrix();
	}
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

unsigned char *buildAlpha(Image *img)
{
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a, b, c;
	unsigned char t0 = *(data + 0);
	unsigned char t1 = *(data + 1);
	unsigned char t2 = *(data + 2);
	for (i = 0; i < img->width * img->height * 3; i += 3)
	{
			a = *(data + 0);
			b = *(data + 1);
			c = *(data + 2);
			*(ptr + 0) = a;
			*(ptr + 1) = b;
			*(ptr + 2) = c;
			*(ptr + 3) = 1;
			if (a == t0 && b == t1 && c == t2)
					*(ptr + 3) = 0;
			ptr += 4;
			data += 3;
	}
	return newdata;
}