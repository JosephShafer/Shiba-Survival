#include <GL/glx.h>
#include "fonts.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>

using namespace std;


class PowerUp{
    public:
        float position[2];
        int status;
        void drawPowerUp();
        void shibaCollision(int);
        PowerUp(float,float);
};

extern std::vector<PowerUp> powerups;

void spawnPowerUp(int, float, flaot);
void destroyPowerUp(int);
void renderPowerUps();
