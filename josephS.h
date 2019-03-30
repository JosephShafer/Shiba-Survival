#ifndef joseph_h
#define joseph_h

#include<vector>
#include <GL/glx.h>
#include "fonts.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
using namespace std;

typedef float Vec[3];

extern void josephS(float, float, GLuint);
void enemyGetResolution(float, float);

class Enemy{
public:
    float position[2];
    float velocity[2];
    float sideLength;
    void drawEnemy();
    void updatePosition(float, float, float, float, int);
    Enemy(float, float);
};

extern std::vector<Enemy> enemies;

void createEnemy(int, float, float);
void destroyEnemy(int);
void renderEnemies();
void updateAllPosition(float, float, float, float);
void cleanupEnemies();


#endif

