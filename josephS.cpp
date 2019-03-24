//Program: josephS.cpp
//Author:  Joseph Shafer
//Date:    2/15/2019

#include <GL/glx.h>
#include "fonts.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
using namespace std;

typedef float Vec[3];

void josephS(float x, float y, GLuint textid)
{
    //show name
    Rect name;
    name.left = x + 90;
    name.bot = y;
    name.center = 0;
    ggprint16(&name, 16, 0x00ffff00, "Joseph Shafer");
       
    glColor3ub(225,225,225);
    float wid = 60.0f;
    //Old code that was used to rotate picture, may use later
    //static float angle = 0.0f;
    //wid += sin(angle) * 10;
    //float fx = (float)x;
    //float fy = (float)y;
    //float a = 0;
    //a += sin(angle);
    //fx += a;
    //angle += 0.2f;
    glPushMatrix();
    glTranslatef(x, y, 0);
    //glRotatef(a, 0, 0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,  wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();
    glPopMatrix();
}


/*
Ideas related to enemies:
    1. move toward the ship at different rates
    2. Will Have different sprites for different enemys
    3. Unlike asteroids, needs to be continually created. Shouldn't exceed a max number. 
    4. Could make a max number of enemies depending on level?
*/

class Enemy{
public:
    float position[2];
	float velocity[2];
    void drawEnemy();
	void setStartingPosition();
    void updatePosition(float, float, float, float);
	void createEnemy();
	
	Enemy();

};

vector<Enemy> enemies;

Enemy::Enemy(){
	static int spawnPoint = 0;
	switch(spawnPoint){
		case 0:
			position[0] = (rand() % 1920);
			position[1] = (rand() % 1);
			break;
		case 1:
			position[0] = (rand() % 1920);
			position[1] = (rand() % 1)  + 1000;
			break;
		case 2:
			position[0] = (rand() % 1);
			position[1] = (rand() % 1080);
			break;
		case 3:
			position[0] = (rand() % 1)  +1920;
			position[1] = (rand() % 1080);
			break;
	}

	spawnPoint++;
	if(spawnPoint == 4)
		spawnPoint = 0;
	
	velocity[0] = 0;
	velocity[1] = 0;
};

void Enemy::drawEnemy()
{
        // square until we add sprites
        
        float sideLength = 3.0f;
         glColor3f(1.0f, 1.0f, 1.0f);
         glBegin(GL_POLYGON);
            glVertex2f(-sideLength, sideLength);
            glVertex2f(sideLength, sideLength);
            glVertex2f(sideLength, -sideLength);
            glVertex2f(-sideLength, -sideLength);
        glEnd();
}


void Enemy::updatePosition(float Xposition, float Yposition, float xWinResolution, float yWinResolution)
{
		float speed = .07 * (rand() % 2);

    	if(position[0] < Xposition)
			velocity[0] += speed;
		if(position[0] > Xposition)
			velocity[0] -= speed;
		if(position[1] < Yposition)
			velocity[1] += speed;
		if(position[1] > Yposition)
			velocity[1] -= speed;
		

		if(position[0] < 0 || position[0] > xWinResolution){
			velocity[0] *= -1;
		}
		if(position[1] < 0 || position[1] > yWinResolution){
			velocity[1] *= -1;
		}
		position[0] = position[0] + velocity[0];
		position[1] = position[1] + velocity[1];   
		   
}
 
void createEnemy(){
	enemies.push_back(Enemy());
}

void destroyEnemy(int position){
	if(enemies.size() > 0)
		enemies.erase(enemies.begin() + position);
}

void renderEnemies(){
	for(int i = 0 ; i < enemies.size(); i++){
	glPushMatrix();
		glTranslated(enemies[i].position[0], enemies[i].position[1], 0);
		//printf(" %d: %f %f\n", i, enemies[i].position[0], enemies[i].position[1]);
		//enemies[i].updatePosition(g.ship.pos[0], g.ship.pos[1], gl->xres, gl->yres);
		enemies[i].drawEnemy();
	glPopMatrix();
	}
}



// void createEnemies(vector<Enemy> &EnemyInstance, int VecSize){
// 	for(int i = 0; i < VecSize; i++){
// 		EnemyInstance.push_back(Enemy());

// }



//Things I may use
/*
class Asteroid {
public:
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
public:
	Asteroid() {
		prev = NULL;
		next = NULL;
	}
};
*/


/**
 Old Asteroid stuff to mess with
From game():
//build 10 asteroids...
		// for (int j=0; j<10; j++) {
		// 	Asteroid *a = new Asteroid;
		// 	a->nverts = 8;
		// 	a->radius = rnd()*80.0 + 40.0;
		// 	Flt r2 = a->radius / 2.0;
		// 	Flt angle = 0.0f;
		// 	Flt inc = (PI * 2.0) / (Flt)a->nverts;
		// 	for (int i=0; i<a->nverts; i++) {
		// 		a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
		// 		a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
		// 		angle += inc;
		// 	}
		// 	a->pos[0] = (Flt)(rand() % gl->xres);
		// 	a->pos[1] = (Flt)(rand() % gl->yres);
		// 	a->pos[2] = 0.0f;
		// 	a->angle = 0.0;
		// 	a->rotate = rnd() * 4.0 - 2.0;
		// 	a->color[0] = 0.8;
		// 	a->color[1] = 0.8;
		// 	a->color[2] = 0.7;
		// 	a->vel[0] = (Flt)(rnd()*2.0-1.0);
		// 	a->vel[1] = (Flt)(rnd()*2.0-1.0);
		// 	//std::cout << "asteroid" << std::endl;
		// 	//add to front of linked list
		// 	a->next = ahead;
		// 	if (ahead != NULL)
		// 		ahead->prev = a;
		// 	ahead = a;
		// 	++nasteroids;
		// }


//-------------------------------------------------------------------------
	//Draw the asteroids
	/**
	{
		Asteroid *a = g.ahead;
		while (a) {
			//Log("draw asteroid...\n");
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
			//Log("%i verts\n",a->nverts);
			for (int j=0; j<a->nverts; j++) {
				glVertex2f(a->vert[j][0], a->vert[j][1]);
			}
			glEnd();
			//glBegin(GL_LINES);
			//	glVertex2f(0,   0);
			//	glVertex2f(a->radius, 0);
			//glEnd();
			glPopMatrix();
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(a->pos[0], a->pos[1]);
			glEnd();
			a = a->next;
		}
	}
	**/
	//-------------------------------------------------------------------------

    /*
void deleteAsteroid(Game *g, Asteroid *node)
{
	//Remove a node from doubly-linked list
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ahead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ahead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}
*/


//Could be used to maybe make an enemy that splits into multiple ?
/*
void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0;
	Flt r2 = ta->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
	ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0;
	ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
	ta->color[0] = 0.8;
	ta->color[1] = 0.8;
	ta->color[2] = 0.7;
	ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
	ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
	//std::cout << "frag" << std::endl;
}
*/

//Update asteroid positions
	/*
	Asteroid *a = g.ahead;
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -1000.0) {
			a->pos[0] += (float)gl->xres+200;
		}
		else if (a->pos[0] > (float)gl->xres+1000) {
			a->pos[0] -= (float)gl->xres+200;
		}
		else if (a->pos[1] < -1000.0) {
			a->pos[1] += (float)gl->yres+200;
		}z
		else if (a->pos[1] > (float)gl->yres+1000) {
			a->pos[1] -= (float)gl->yres+200;
		}
		a->angle += a->rotate;
		a = a->next;
	}

    */
