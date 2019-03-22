#ifndef joseph_h
#define joseph_h

#include<vector>

typedef float Vec[3];

extern void josephS(float, float, GLuint);


class Enemy{
public:
    float position[2];
    float velocity[2];
    
    void drawEnemy();
    void updatePosition(float, float, float, float);
    Enemy();
};

void createEnemies(std::vector<Enemy> &, int vecSize);

#endif

