#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"

void josephS(int x, int y, GLuint textid)
{
    glColor3ub(225,225,225);
    static float angle = 0.0f;
    static int wid = 30;
    wid += sin(angle) * 10;
    float fx = (float)x;
    float fy = (float)y;
    float a = 0;
    a += sin(angle) * 10.0f;
    fx += a;
    angle += 0.2f;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glRotatef(a, 0, 0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1,0f); glVertex2i(-wid, -wid);
        glTexCoord2f(0.0f,1,0f); glVertex2i(-wid, -wid);
        glTexCoord2f(0.0f,1,0f); glVertex2i(-wid, -wid);    
	    glTexCoord2f(0.0f,1,0f); glVertex2i(-wid, -wid);
    glEnd();
    glPopMatrix();
}
