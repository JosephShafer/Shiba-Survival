//Program: josephS.cpp
//Author:  Joseph Shafer
//Date:    2/15/2019

#include "josephS.h"


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
float gameXresolution;
float gameYresolution;

void enemyGetResolution(float Xres, float Yres)
{
	gameXresolution = Xres;
	gameYresolution = Yres;
}

/*
dogs are scared of
balloons
trashbags
garden hoses
spray bottles
flea/bugs
other dogs


chocolate?
*/

//=============================================================
//Enemy functions
//=============================================================


/*
Never want to spawn on top of ship, would not be fair to player
get shiba position, 
can't spawn past edge, 


*/

vector<Enemy> enemies;
int Enemy::enemiesHitShiba = 0; // static variable

Enemy::Enemy(float shibaXPosition, float shibaYPosition)
{

	velocity[0] = 0;
	velocity[1] = 0;
	health = 100;
	speed = .05;

	//enemies spawn
	int spawnchoice = (rand() % 4);
	int spaceAway = 100;
	bool enemySpawned = false;

	//spawn top right area of game area
	while(!enemySpawned){
		if (spawnchoice == 0){
			if((shibaXPosition + spaceAway) < gameXresolution && (shibaYPosition + spaceAway) < gameYresolution){
				position[0] = (rand() % ((int)(gameXresolution - spaceAway + 1 - shibaXPosition)))
								+ shibaXPosition + spaceAway;
				position[1] = (rand() % ((int)(gameYresolution - spaceAway + 1 - shibaYPosition)))
								+ shibaYPosition + spaceAway;
				enemySpawned = true;
				#ifdef debug
					if(position[0] > gameXresolution || position[1] > gameYresolution){
						printf("Top right Error: %f, %f\n", position[0], position[1]);
					}
				#endif

			}else{
				spawnchoice++;
			}
		}
		//spawn top left area of shiba
		if (spawnchoice == 1){
			if((shibaXPosition) > 0 && (shibaYPosition + spaceAway) < gameYresolution){
				position[0] = (rand() % ((int)(shibaXPosition - spaceAway + 1)));
				position[1] = (rand() % ((int)(gameYresolution - spaceAway + 1 - shibaYPosition)))
								+ shibaYPosition + spaceAway;
				enemySpawned = true;
				#ifdef debug
					if(position[0] < 0 || position[1] > gameYresolution){
						printf("Top Left Error: %f, %f\n", position[0], position[1]);
					}
				#endif
			} else{
				spawnchoice++;
			}
		}

		// rand() % (max_number + 1 - minimum_number) + minimum_number
		// bottom right
		if (spawnchoice == 2){
			if((shibaXPosition + spaceAway) < gameXresolution && (shibaYPosition > 0)){
				position[0] = (rand() % ((int)(gameXresolution - spaceAway + 1 - shibaXPosition)))
								+ shibaXPosition + spaceAway;
				position[1] = (rand() % ((int)(shibaYPosition - spaceAway + 1 )));
				enemySpawned = true;
				#ifdef debug
					if(position[0] > gameXresolution || position[1] < 0){
						printf("Bottom Right Error: %f, %f\n", position[0], position[1]);
					}
				#endif
			} else{
				spawnchoice++;
			}
		}

		// bottom left
		if (spawnchoice == 3){
			if((shibaXPosition - spaceAway) > 0 && (shibaYPosition - spaceAway > 0)){
				position[0] = (rand() % ((int)(shibaXPosition - spaceAway + 1)));
				position[1] = (rand() % ((int)(shibaYPosition - spaceAway + 1 )));
				enemySpawned = true;
			#ifdef debug
				if(position[0] < 0 || position[1] < 0){
					printf("Bottom Left Error: %f, %f\n", position[0], position[1]);
				}
			#endif
			}else{
				spawnchoice = 0;
			}
		}
	}	//end while
};



void Enemy::drawEnemy()
{
        // square until we add sprites
         sideLength = 10.0f;
         glColor3f(1.0f, 1.0f, 1.0f);
         glBegin(GL_POLYGON);
            glVertex2f(-sideLength, sideLength);
            glVertex2f(sideLength, sideLength);
            glVertex2f(sideLength, -sideLength);
            glVertex2f(-sideLength, -sideLength);
        glEnd();
}

void Enemy::updatePosition(float shibaXposition, float shibaYposition, int indexOfEnemy)
{
	

	if(position[0] < shibaXposition)
		velocity[0] += speed;
	if(position[0] > shibaXposition)
		velocity[0] -= speed;
	if(position[1] < shibaYposition)
		velocity[1] += speed;
	if(position[1] > shibaYposition)
		velocity[1] -= speed;
	
	position[0] = position[0] + velocity[0];
	position[1] = position[1] + velocity[1];
	
	if((position[0] + sideLength) < sideLength) 
		velocity[0] *= -1;
	if((position[0] - sideLength) > gameXresolution)
		velocity[0] *= -1;
	if((position[1] + sideLength) < sideLength)
		velocity[1] *= -1;
	if((position[1] - sideLength) > gameYresolution)
		velocity[1] *= -1;

	
	//weird formatting just makes it easier for me to see for now
	if(     (((position[0] - sideLength) < shibaXposition)  && ((position[0] + sideLength) > shibaXposition))
															  &&
			(((position[1] - sideLength) < shibaYposition)  && ((position[1] + sideLength) > shibaYposition))
	) {
		shibaCollision(indexOfEnemy);
	}

	#ifdef debug
		if(position[0] < -30)
			printf("Enemy position error: Left of game window error\n");
		if(position[0] > gameXresolution + 30)
			printf("Enemy position error: right of game window error\n");
		if(position[1] < -30)
			printf("Enemy position error: below game window error\n");
		if(position[1] > gameYresolution + 30)
			printf("Enemy position error: Above game window error\n");
	#endif

}

void Enemy::shibaCollision(int indexOfEnemy)
{
	enemies.erase(enemies.begin()+indexOfEnemy);
	enemiesHitShiba++;
}

void Enemy::takeDamage(int damageDone)
{
	health -= damageDone;
}



//=============================================================
// Functions used in Main file
//=============================================================

void createEnemy(int numToCreate, float shibaXPosition, float shibaYPosition)
{
	for(int i = 0; i < numToCreate; i++)
		enemies.push_back(Enemy(shibaXPosition, shibaYPosition));
}
// destroys an element in a vector by it's index
void destroyEnemy(int index)
{
	if(enemies.size() > 0)
		enemies.erase(enemies.begin() + index);
}

void renderEnemies()
{
	for(unsigned int i = 0; i < enemies.size(); i++) {
	glPushMatrix();
		glTranslated(enemies[i].position[0], enemies[i].position[1], 0);
		//printf(" %d: %f %f\n", i, enemies[i].position[0], enemies[i].position[1]);
		//enemies[i].updatePosition(g.ship.pos[0], g.ship.pos[1], gl->xres, gl->yres);
		enemies[i].drawEnemy();
	glPopMatrix();
	}
}

void updateAllPosition(float shibaXposition, float shibaYposition)
{
	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i].updatePosition(shibaXposition, shibaYposition, i);
	}
}

void cleanupEnemies()
{
	while(enemies.size() != 0){
		enemies.pop_back();
	}
}

/**
 * 
Image enemySprite[1] = {"possibleEnemy.gif"};
GLuint EnemyTexture;

void enemyInit_openGL(){
	int w = enemySprite[0].width;
	int h = enemySprite[0].height;
	glBindTexture(GL_TEXTURE_2D, EnemyTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *EnemyData = buildAlphaData(&enemySprite[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, EnemyData);
}


// void createEnemies(vector<Enemy> &EnemyInstance, int VecSize){
// 	for(int i = 0; i < VecSize; i++){
// 		EnemyInstance.push_back(Enemy());

// }
*/


