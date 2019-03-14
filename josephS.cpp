//Program: josephS.cpp
//Author:  Joseph Shafer
//Date:    2/15/2019

#include <GL/glx.h>
#include "fonts.h"

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
    Enemy(){
        position[0] = 300;
        position[1] = 300;
    }
    void drawEnemy();
    void updatePosition(Vec, Vec);

};



void Enemy::drawEnemy(){
        // square until we add sprites
        
        float sideLength = 10.0f;
         glColor3f(1.0f, 1.0f, 1.0f);
         glBegin(GL_POLYGON);
            glVertex2f(-sideLength, sideLength);
            glVertex2f(sideLength, sideLength);
            glVertex2f(sideLength, -sideLength);
            glVertex2f(-sideLength, -sideLength);
        glEnd();
}


void Enemy::updatePosition(Vec, Vec){

    //position[0] -= 3;
    //position[1] -= 2;

}



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
		if (a->pos[0] < -100.0) {
			a->pos[0] += (float)gl->xres+200;
		}
		else if (a->pos[0] > (float)gl->xres+100) {
			a->pos[0] -= (float)gl->xres+200;
		}
		else if (a->pos[1] < -100.0) {
			a->pos[1] += (float)gl->yres+200;
		}z
		else if (a->pos[1] > (float)gl->yres+100) {
			a->pos[1] -= (float)gl->yres+200;
		}
		a->angle += a->rotate;
		a = a->next;
	}

    */
