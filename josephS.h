#ifndef joseph_h
#define joseph_h

#include <GL/glx.h>
#include "fonts.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "amberZ.h"
#include "Image.h"
#define numEnemyImages 3
using namespace std;

typedef float Vec[3];

class JoeyGlobal{
public:

	float gameXresolution;
	float gameYresolution;
    GLuint textureArray[3];

    float * shibaXPtr;
    float * shibaYPtr;
    
    static JoeyGlobal *instance;
    static JoeyGlobal *getInstance(){
        if(!instance){
            instance = new JoeyGlobal;
        }
        return instance;
    }
    JoeyGlobal(){
        gameXresolution = 1366;
		gameYresolution = 766;
    }
};


void enemyGetResolution(float, float);

class Enemy{
public:

    float shibaX;
    float shibaY;
    float position[2];
    float velocity[2];
    float sideLength;
    float speed;
    int health;
    GLuint textureUsed;
    Image* imageUsed;
    int imageIndex;
    bool splitter = false;

    
    SpriteTimer timer;
    void splitterSpawn(float, float);
    void spawn(float, float);
    float getShibaXListener();
    float getShibaYListener();
    void setShibaXListener(float);
    void setShibaYListener(float);
    void drawEnemy();
    void updatePosition(float, float, int);
    void takeDamage(int);
    void setTexture();
    Enemy();
};

class EnemyControl{
    public:
        vector<Enemy> enemies;
        void shibaCollision(int);
        void createEnemy(int, float, float);
        void destroyEnemy(int);
        void renderEnemies();
        void updateAllPosition(float, float);
        void cleanupEnemies();
        bool bulletHitEnemy(float, float);
        void primeSpawner(int, float, float);
        void createSplitEnemy(float, float);
        //EnemyControl();
};

class Lives{
    public:
        int currentLives;
        void setLives(int);
        int getLives();
        void changeLives(int);
        void livesTextDisplay();
    Lives();
};

class Score{
    public:
        float currentScore;
        void setScore(float);
        float getScore();
        void changeScore(float);
        float calculateScore(float);
        void textScoreDisplay();
    Score();
};

extern Lives numLivesLeft;
extern Score scoreObject;
extern EnemyControl enemyController;
//int Enemy::enemiesHitShiba = 0;
extern Image enemyImages[3];
//extern std::vector<Enemy> enemies;


//void scoreCalculator(float, float * score);
void getTexturesFunction(GLuint);
extern void josephS(float, float, GLuint);


#endif

