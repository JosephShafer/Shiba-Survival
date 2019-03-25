#ifndef joseph_h
#define joseph_h

#include<vector>

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
    Enemy();
};
extern std::vector<Enemy> enemies;

void createEnemy(int);
void destroyEnemy(int);
void renderEnemies();
void updateAllPosition(float, float, float, float);

#endif

