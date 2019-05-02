// File: danL.cpp
// Author: Daniel Leinker
// Functions for software engineering 3350


#include <GL/glx.h>
#include "fonts.h"

void danL(int x, int y, GLuint textid)
{
    // Show picture
    float wid = 120.0f;
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef((float)x, (float)y, 0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid/2, -wid/2);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid/2,  wid/2);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid/2,  wid/2);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid/2, -wid/2);
    glEnd();
    glPopMatrix();
    // Show name
    Rect r;
    r.bot = y;
    r.left = x + 90;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffff00, "Dan Leinker");
}

