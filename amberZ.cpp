// Program: amberZ.cpp
// Author:  Amber Zaragoza
// Personal functions for Shiba Survival

#include "amberZ.h"

// Structs and classes for SSD timer
void Timer::startTimer()
{
	startTime = std::chrono::system_clock::now();
	isRunning = true;
}

void Timer::stopTimer()
{
	endTime = std::chrono::system_clock::now();
	isRunning = false;
}

double Timer::getElapsedMilliseconds()
{
	std::chrono::time_point<std::chrono::system_clock> endTime;
	if (isRunning) {
		endTime = std::chrono::system_clock::now();
	} else {
		endTime = endTime;
	}
	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}

double Timer::getElapsedSeconds()
{
	return getElapsedMilliseconds() / 1000.0;
}

double Timer::getElapsedMinutes()
{
	return getElapsedSeconds() / 60.0;
}

Timer gameTimer;

SSD::SSD()
{
	screen = std::bitset<7>("1111110");
}

void SSD::updateDisplay(int num)
{
	switch (num) {
		case 0:
			screen = std::bitset<7>("1111110");
			break;
		case 1:
			screen = std::bitset<7>("0110000");
			break;
		case 2:
			screen = std::bitset<7>("1101101");
			break;
		case 3:
			screen = std::bitset<7>("1111001");
			break;
		case 4:
			screen = std::bitset<7>("0110011");
			break;
		case 5:
			screen = std::bitset<7>("1011011");
			break;
		case 6:
			screen = std::bitset<7>("0011111");
			break;
		case 7:
			screen = std::bitset<7>("1110000");
			break;
		case 8:
			screen = std::bitset<7>("1111111");
			break;
		case 9:
			screen = std::bitset<7>("1110011");
			break;
		default:
			screen = std::bitset<7>("1001000");
			break;
	}
}

void SSD::renderSSD()
{
	if (screen[6]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 100.0f);
		glVertex2f(0.0f, 110.0f);
		glVertex2f(40.0f, 110.0f);
		glVertex2f(40.0f, 100.0f);
		glEnd();
	}
	if (screen[5]) {
		glBegin(GL_POLYGON);
		glVertex2f(30.0f, 60.0f);
		glVertex2f(30.0f, 100.0f);
		glVertex2f(40.0f, 100.0f);
		glVertex2f(40.0f, 60.0f);
		glEnd();
	}
	if (screen[4]) {
		glBegin(GL_POLYGON);
		glVertex2f(30.0f, 10.0f);
		glVertex2f(30.0f, 50.0f);
		glVertex2f(40.0f, 50.0f);
		glVertex2f(40.0f, 10.0f);
		glEnd();
	}
	if (screen[3]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 10.0f);
		glVertex2f(40.0f, 10.0f);
		glVertex2f(40.0f, 0.0f);
		glEnd();
	}
	if (screen[2]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 10.0f);
		glVertex2f(0.0f, 50.0f);
		glVertex2f(10.0f, 50.0f);
		glVertex2f(10.0f, 10.0f);
		glEnd();
	}
	if (screen[1]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 60.0f);
		glVertex2f(0.0f, 100.0f);
		glVertex2f(10.0f, 100.0f);
		glVertex2f(10.0f, 60.0f);
		glEnd();
	}
	if (screen[0]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 50.0f);
		glVertex2f(0.0f, 60.0f);
		glVertex2f(40.0f, 60.0f);
		glVertex2f(40.0f, 50.0f);
		glEnd();
	}
}

void SSD::renderColon()
{
	if (screen[6]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 90.0f);
		glVertex2f(0.0f, 100.0f);
		glVertex2f(10.0f, 100.0f);
		glVertex2f(10.0f, 90.0f);

		glEnd();
	}
	if (screen[3]) {
		glBegin(GL_POLYGON);
		glVertex2f(0.0f, 20.0f);
		glVertex2f(0.0f, 10.0f);
		glVertex2f(10.0f, 10.0f);
		glVertex2f(10.0f, 20.0f);
		glEnd();
	}
}

SSD min1, min2, col, sec1, sec2;

void drawTimer(int xres)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslated((xres / 2) - 100, 595.0f, 0.0f);
	min1.renderSSD();
	glPopMatrix();	
	glPushMatrix();
	glTranslated((xres / 2) - 50, 595.0f, 0.0f);
	min2.renderSSD();
	glPopMatrix();
	glPushMatrix();
	glTranslated((xres / 2), 595.0f, 0.0f);
	col.renderColon();
	col.updateDisplay(10);
	glPopMatrix();
	glPushMatrix();
	glTranslated((xres / 2) + 20, 595.0f, 0.0f);
	sec1.renderSSD();
	glPopMatrix();
	glPushMatrix();
	glTranslated((xres / 2) + 70, 595.0f, 0.0f);
	sec2.renderSSD();
	glPopMatrix();
}

void updateTimer(int min, int sec)
{
	if (min > 9) {
		min1.updateDisplay(min / 10);
		min2.updateDisplay(min % 10);
	} else {
		min1.updateDisplay(0);
		min2.updateDisplay(min);
	}
	if (sec > 9) {
		sec1.updateDisplay(sec / 10);
		sec2.updateDisplay(sec % 10);
	} else {
		sec1.updateDisplay(0);
		sec2.updateDisplay(sec);
	}
}

void drawSprite(GLuint texture, Image& sprite, float width, float height, float xpos, float ypos)
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255, 255, 255, 255);
	int ix = sprite.frame % sprite.columns;
	int iy = sprite.animation;
	float tx = (float) ix / sprite.columns;
	float ty = (float) iy / sprite.rows;
	float swidth = (float) 1.0 / sprite.columns;
	float sheight = (float) 1.0 / sprite.rows;
	if (sprite.frame >= sprite.columns)
		iy = 1;
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(tx, ty + sheight);
	glVertex2i(xpos - width, ypos - height);
	glTexCoord2f(tx, ty);
	glVertex2i(xpos - width, ypos + height);
	glTexCoord2f(tx + swidth, ty);
	glVertex2i(xpos + width, ypos + height);
	glTexCoord2f(tx + swidth, ty + sheight);
	glVertex2i(xpos + width, ypos - height);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}

void updateFrame(Image& sprite) {
	++sprite.frameCounter %= (sprite.columns + 1);
	if (sprite.frameCounter == sprite.columns) {
		++sprite.frame;
	}
	if (sprite.frame >= sprite.columns)
		sprite.frame = 0;
}

// Displays my name and photo on the credits screen
void amberZ(int x, int y, GLuint textureId)
{
	// Show picture
	float wid = 120.0f;
	glColor3ub(255, 255, 255);
	glPushMatrix();
	glTranslatef((float)x, (float)y, 0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid / 2, -wid / 2);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid / 2, wid / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(wid / 2, wid / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(wid / 2, -wid / 2);
	glEnd();
	glPopMatrix();
	// Show name
	Rect r;
	r.bot = y;
	r.left = x + 90;
	r.center = 0;
	ggprint16(&r, 16, 0x00ffff00, "Amber Zaragoza");
}

// Store user scores to a CSV file
void storeScore(char user[], int score)
{
	printf("%s: %d", user, score);
}