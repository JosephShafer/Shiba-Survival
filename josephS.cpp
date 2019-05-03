///Program: josephS.cpp
//Author:  Joseph Shafer
//Date:    2/15/2019

#include "josephS.h"



JoeyGlobal *JoeyGlobal::instance = 0;
JoeyGlobal *JSglobalVars = JSglobalVars->getInstance();


void enemyGetResolution(float Xres, float Yres)
{
	JSglobalVars->gameXresolution = Xres;
	JSglobalVars->gameYresolution = Yres;
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

vector<Enemy> enemies;
Lives numLivesLeft;
Score scoreObject;
//remember to update h file, global, and the modulus

int Enemy::enemiesHitShiba = 0; // static variable

Image enemyImages[numEnemyImages] = {
						Image("./images/Cat-Sprites.png", 4, 4),
						Image("./images/Doctor_Left.png", 1, 4),
						Image("./images/Shiba-Sprites.png", 9, 4)
						//Image("./images/Cat-Sprites.png", 4, 4)
						};

Enemy::Enemy()
{
	velocity[0] = 0;
	velocity[1] = 0;
	sideLength = float(rand() % 50 + 5);
	health = 100;
	speed = .01;
	imageIndex = rand() % numEnemyImages;
	textureUsed = JSglobalVars->textureArray[imageIndex];
	imageUsed = &enemyImages[imageIndex];
	//shibaX = 0;
	//shibaY = 0;
};

float Enemy::getShibaXListener(){
	return shibaX;
};

void Enemy::setShibaXListener(float shibaXposition){
	shibaX = shibaXposition;
};

float Enemy::getShibaYListener(){
	return shibaY;
};

void Enemy::setShibaYListener(float shibaYposition){
	shibaY = shibaYposition;
};

void Enemy::spawn(float Xposition, float Yposition)
{
	int spawnchoice = (rand() % 4);
	int spaceAway = 100;
	bool enemySpawned = false;

	if(sideLength > 45){
		splitter = true;
	}

	//spawn top right area of game area
	while(!enemySpawned) {
		if (spawnchoice == 0) {
			if((Xposition + spaceAway) < JSglobalVars->gameXresolution && (Yposition + spaceAway) < JSglobalVars->gameXresolution) {
				position[0] = (rand() % ((int)(JSglobalVars->gameXresolution - spaceAway + 1 - Xposition)))
								+ Xposition + spaceAway;
				position[1] = (rand() % ((int)(JSglobalVars->gameXresolution - spaceAway + 1 - Yposition)))
								+ Yposition + spaceAway;
				enemySpawned = true;
				#ifdef DEBUG
					if(position[0] > JSglobalVars->gameXresolution || position[1] > JSglobalVars->gameXresolution) {
						printf("Top right Error: %f, %f\n", position[0], position[1]);
					}
				#endif

			}else{
				spawnchoice++;
			}
		}
		//spawn top left area of shiba
		if (spawnchoice == 1) {
			if((Xposition) > 0 && (Yposition + spaceAway) < JSglobalVars->gameXresolution) {
				position[0] = (rand() % ((int)(Xposition - spaceAway + 1)));
				position[1] = (rand() % ((int)(JSglobalVars->gameXresolution - spaceAway + 1 - Yposition)))
								+ Yposition + spaceAway;
				enemySpawned = true;
				#ifdef DEBUG
					if(position[0] < 0 || position[1] > JSglobalVars->gameXresolution) {
						printf("Top Left Error: %f, %f\n", position[0], position[1]);
					}
				#endif
			} else{
				spawnchoice++;
			}
		}

		// rand() % (max_number + 1 - minimum_number) + minimum_number
		// bottom right
		if (spawnchoice == 2) {
			if((Xposition + spaceAway) < JSglobalVars->gameXresolution && (Yposition > 0)) {
				position[0] = (rand() % ((int)(JSglobalVars->gameXresolution - spaceAway + 1 - Xposition)))
								+ Xposition + spaceAway;
				position[1] = (rand() % ((int)(Yposition - spaceAway + 1 )));
				enemySpawned = true;
				#ifdef DEBUG
					if(position[0] > JSglobalVars->gameXresolution || position[1] < 0) {
						printf("Bottom Right Error: %f, %f\n", position[0], position[1]);
					}
				#endif
			} else{
				spawnchoice++;
			}
		}

		// bottom left
		if (spawnchoice == 3) {
			if((Xposition - spaceAway) > 0 && (Yposition - spaceAway > 0)) {
				position[0] = (rand() % ((int)(Xposition - spaceAway + 1)));
				position[1] = (rand() % ((int)(Yposition - spaceAway + 1 )));
				enemySpawned = true;
			#ifdef DEBUG
				if(position[0] < 0 || position[1] < 0) {
					printf("Bottom Left Error: %f, %f\n", position[0], position[1]);
				}
			#endif
			}else{
				spawnchoice = 0;
			}
		}
	}	//end while

	//cout << position[0] << " ";
	//cout << position[1] << endl;
};



void Enemy::drawEnemy()
{
        drawSprite(textureUsed, *imageUsed, sideLength, sideLength, position[0], position[1]);
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
	
	if((position[0] - sideLength) <= 0) {
		position[0] = sideLength;
		velocity[0] *= -1;
	}
	if((position[0] + sideLength) >= JSglobalVars->gameXresolution) {
		position[0] = JSglobalVars->gameXresolution - sideLength;
		velocity[0] *= -1;
	}
	if((position[1] - sideLength) <= 0) {
		position[1] = sideLength;
		velocity[1] *= -1;
	}
	if((position[1] + sideLength) >= JSglobalVars->gameYresolution) {
		position[1] = JSglobalVars->gameYresolution - sideLength;
		velocity[1] *= -1;
	}

	if((( (position[0] - sideLength) < shibaXposition)
		  && 
		  ((position[0] + sideLength) > shibaXposition))
			&&
			(((position[1] - sideLength) < shibaYposition)
			  && 
			  ((position[1] + sideLength) > shibaYposition))) {
		shibaCollision(indexOfEnemy);
		numLivesLeft.changeLives(-1);
	}

	#ifdef DEBUG
		if(position[0] < -30)
			printf("Enemy position error: Left of game window error\n");
		if(position[0] > JSglobalVars->gameXresolution + 30)
			printf("Enemy position error: right of game window error\n");
		if(position[1] < -30)
			printf("Enemy position error: below game window error\n");
		if(position[1] > JSglobalVars->gameXresolution + 30)
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
//		Score Display
//=============================================================

Lives::Lives()
{
	currentLives= 3;
};

int Lives::getLives()
{
	return currentLives;
};

void Lives::setLives(int changeValue)
{
	currentLives = changeValue;
};

void Lives::changeLives(int difference)
{
	currentLives += difference;
};

void livesTextDisplay()
{
	Rect livesLeft;
    livesLeft.left = JSglobalVars->gameXresolution * .80;
    livesLeft.bot = JSglobalVars->gameYresolution * .925;
    livesLeft.center = 0;
	ggprint16(&livesLeft, 16, 0x00ffff00, "Lives: %d", numLivesLeft.getLives());
}

Score::Score()
{
	currentScore = 0;
};

float Score::calculateScore(float scoreIncrease)
{
	return 100000*(1/scoreIncrease);
};

void Score::setScore(float scoreSet)
{
	currentScore += scoreSet;
};

float Score::getScore()
{
	return currentScore;
};

void textScoreDisplay()
{
	Rect score;
    score.left = JSglobalVars->gameXresolution * .80;
    score.bot = JSglobalVars->gameYresolution * .95;
    score.center = 0;
    ggprint16(&score, 16, 0x00ffff00, "Score: %019.0f", scoreObject.getScore());
}
//=============================================================
// Functions used in Main file
//========================================================ma=====

void createEnemy(int numToCreate, float shibaXPosition, float shibaYPosition)
{
	for(int i = 0; i < numToCreate; i++){
		enemies.push_back(Enemy());
		//cout << shibaXPosition << endl;
		enemies.back().setShibaXListener(shibaXPosition);
		enemies.back().setShibaYListener(shibaYPosition);


		enemies.back().spawn(enemies.back().getShibaXListener(), enemies.back().getShibaYListener());

		//cout << enemies[enemies.size()].position[0] << " " <<enemies << endl; 
	}

	//cout << enemies[4].position[0] << " " << enemies[4].position[1] << endl;
	//for(int i = 0; i < numToCreate; i++){
	//	enemies[i].spawn();
	//}
		
}

// destroys an element in a vector by it's index
void destroyEnemy(int index)
{
	if(enemies.size() > 0){
		if(enemies[index].splitter == true){
			//createEnemy(10, enemies[index].getShibaX(), enemies[index].getShibaY());
			numLivesLeft.changeLives(1);
		}
		enemies.erase(enemies.begin() + index);
	}
}

void renderEnemies()
{
	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i].drawEnemy();
		updateFrame(*(enemies[i].imageUsed), enemies[i].timer, 3.0);
	}
}

void updateAllPosition(float shibaXposition, float shibaYposition)
{
	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i].updatePosition(shibaXposition, shibaYposition, i);
		// TODO MOVE TO OTHER FUNCTION MAYBE
		if(enemies[i].health < 1) {
			destroyEnemy(i);
		}
		// END TODO
	}
}

void cleanupEnemies()
{
	while(enemies.size() != 0) {
		enemies.pop_back();
	}
}

bool bulletHitEnemy(float bulletX, float bulletY)
{
	bool hit = false;
	for(unsigned int j = 0; j < enemies.size(); j++) {
			if((bulletX - enemies[j].sideLength < enemies[j].position[0]) 
				&&
				(bulletX + enemies[j].sideLength > enemies[j].position[0])
				&&
				(bulletY + enemies[j].sideLength > enemies[j].position[1])
				&&
				(bulletY - enemies[j].sideLength < enemies[j].position[1])
				)
				{

				enemies[j].takeDamage(100);
				hit = true;
				scoreObject.setScore(scoreObject.calculateScore(enemies[j].sideLength));
			}
		}
		return hit;
}

void primeSpawner(int milliseconds, float shibaXposition, float shibaYposition)
{
	int primeArray[] = {101, 103, 107, 109, 113};

	static int currentIndex = sizeof(primeArray) / sizeof(primeArray[0]) - 1;
	static int numToMake = 1;

	int spawnChecker = (milliseconds % primeArray[currentIndex]);
	unsigned int enemyCap = 10;
	if(enemies.size() < 1){
	//	createEnemy(1, shibaXposition, shibaYposition);
		
	}
	if(spawnChecker == 0 && enemies.size() < enemyCap) {
		createEnemy(numToMake, shibaXposition, shibaYposition);
		if (currentIndex > 0) {
			--currentIndex;
		} else{
			currentIndex = sizeof(primeArray) / sizeof(primeArray[0]) - 1;
			numToMake++;
		}
	}

	if(milliseconds % 401 == 0) {
		enemyCap++;
	}

}


void getTexturesFunction(GLuint recievedTexture) {
	 
	static int i = 0;
	JSglobalVars->textureArray[i] = recievedTexture;
	i++;
}


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

