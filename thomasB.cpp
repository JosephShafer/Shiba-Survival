// Program: thomasB.cpp
// Author:  Thomas Basden
// Functions for Shiba Survival

#include <GL/glx.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "fonts.h"
#include "thomasB.h"
#define MAXBUTTONS 5

int nbuttons = 0;
int location = 0;
Button button[MAXBUTTONS];

void thomasB (int x, int y, GLuint textid)
{
	//Show picture
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
	ggprint16(&r, 16, 0xffffffff, "Thomas Basden");
	}

// menu(): function to create the game menu
void menu(GLenum target, GLuint texture, int xres, int yres)
{
	//show the background image
	glBindTexture(target, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres); 
		glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	Rect r;
	nbuttons=0;

	//size, position, & color
	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 50;
	button[nbuttons].r.bot = 560;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
		button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
		button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
		button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Play");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.8f;
	button[nbuttons].color[2] = 0.1f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 50;
	button[nbuttons].r.bot = 440;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
		button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
		button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
		button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "How to play");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.8f;
	button[nbuttons].color[2] = 0.1f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 50;
	button[nbuttons].r.bot = 320;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
		button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
		button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
		button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "High Scores");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.8f;
	button[nbuttons].color[2] = 0.1f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	
	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 50;
	button[nbuttons].r.bot = 200;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
		button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
		button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
		button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Credits");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.8f;
	button[nbuttons].color[2] = 0.1f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	button[nbuttons].r.width = 140;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 50;
	button[nbuttons].r.bot = 80;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot +
		button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left +
		button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot +
		button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Quit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.8f;
	button[nbuttons].color[2] = 0.1f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;

	//draw the buttons
	for (int i=0; i< MAXBUTTONS; i++) {
		glColor3f(1.0f, 1.0f, 0.0f);
			
		if (location == i) {
			glColor3fv(button[i].dcolor);
		} 
		else {
			glColor3fv(button[i].color);
		}
		
		glBegin(GL_QUADS);
			glVertex2i(button[i].r.left,  button[i].r.bot);
			glVertex2i(button[i].r.left,  button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();

		r.left = button[i].r.centerx;
		r.bot  = button[i].r.centery-8;
		r.center = 1;
		
		ggprint16(&r, 0, button[i].text_color, button[i].text);
	}    
}

// howToPlay(): Function to display instructions
void howToPlay(int xres, int yres)
{
	glClear(GL_COLOR_BUFFER_BIT);

	Rect howTo;
	howTo.bot = yres * 0.95f;
	howTo.left = xres/2;
	howTo.center = 0;
	ggprint16(&howTo, 0, 0xffffffff, "How To Play");
	howTo.bot = yres * 0.85f;
	howTo.left = xres / 4;
	ggprint16(&howTo, 0, 0xffffffff, "Use the arrow keys to move.");
	howTo.bot = yres * 0.75f;
	howTo.left = xres / 4;
	ggprint16(&howTo, 0, 0xffffffff, "Press space to shoot.");
	howTo.bot = yres * 0.65f;
	howTo.left = xres / 4;
	ggprint16(&howTo, 0, 0xffffffff, "Goals:");
	howTo.bot = yres * 0.55f;
	howTo.left = xres / 4;
	ggprint16(&howTo, 20, 0xffffffff, "          Avoid the enemies to stay alive");
	howTo.bot = yres * 0.45f;
	howTo.left = xres / 4;
	ggprint16(&howTo, 20, 0xffffffff, "          Shoot the enemies and collect powerups to increase your score");	
}

// Get the first place score
int firstPlace()
{
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
	int highScore = 0;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			name = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
		}
        if (currentScore > highScore){
            highScore = currentScore;
        }
	}
    file.close();
    return highScore;
}

//Get the second place score
int secondPlace(int first)
{
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
	int highScore = 0;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			name = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
            if (currentScore == first){
                currentScore = 0;
            }
		}
        if (currentScore > highScore){
            highScore = currentScore;
        }
	}
    file.close();
    return highScore;
}

//Get the third place score
int thirdPlace(int first, int second)
{
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
	int highScore = 0;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			name = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
            if (currentScore == first || currentScore == second){
                currentScore = 0;
            }
		}
        if (currentScore > highScore){
            highScore = currentScore;
        }
	}
    file.close();
    return highScore;
}

//Get the position of the current player's score on the leader board
int playerRank(int score)
{
	int rank = 1;
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			name = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
            if (currentScore > score){
                rank++;
	        }
		}
	}
    file.close();
    return rank;
}

//get the name of the first place user
std::string firstPlacePlayerName()
{
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
    std::string currentName;
	int highScore = 0;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			currentName = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
	    }    
        if (currentScore > highScore){
            highScore = currentScore;
            name = currentName;
        }
	}
    file.close();
    return name;

}

//get the name of the second place user
std::string secondPlacePlayerName(int first)
{
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
    std::string currentName;
	int highScore = 0;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			currentName = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
            if (currentScore == first){
                currentScore = 0;
            }
		}
        if (currentScore > highScore){
            highScore = currentScore;
            name = currentName;
        }
	}
    file.close();
    return name;
}

//get the name of the third place user
std::string thirdPlacePlayerName(int first, int second)
{
    std::string result;
    std::ifstream file("scores.csv");
	std::string line;
	std::string name;
    std::string currentName;
	int highScore = 0;
    int currentScore = 0;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string value;
		while (std::getline(ss, value, ',')) {
			currentName = value;
			std::getline(ss, value, ',');
			currentScore = stod(value);
            if (currentScore == first || currentScore == second){
                currentScore = 0;
            }
		}
        if (currentScore > highScore){
            highScore = currentScore;
            name = currentName;
        }
	}
    file.close();
    return name;
}

//Print out the game over screen
void gameOver(int xres, int yres, char* user, float score, GLenum target, GLuint texture)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Print the Game Over image
	glBindTexture(target, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(xres/2-300, yres-100);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(xres/2-300, yres-26); 
		glTexCoord2f(1.0f, 0.0f); glVertex2i(xres/2 + 300, yres-26);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(xres/2 + 300, yres-100);
	glEnd();

	char buffer[100];
	int temp = score * 1; 
	snprintf(buffer, sizeof buffer, "%d",temp);

	// print the current player's information
	Rect name;
	name.bot = yres - 300;
	name.left = xres/2 - 200;
	name.center = 0;
	int position = playerRank(score);
	std::string rank = std::to_string(position);
	ggprint16(&name, 0, 0xffffffff, rank.c_str());
	name.left = xres/2;
	ggprint16(&name, 0, 0xffffffff, user);
	name.left = xres/2 + 200;
	ggprint16(&name, 0, 0xffffffff, buffer);

	//read in the top 3 high scores
	int first = firstPlace();
    std::string firstPlace = firstPlacePlayerName();
	int second= secondPlace(first);
    std::string secondPlace = secondPlacePlayerName(first);
	int third = thirdPlace(first, second);
    std::string thirdPlace = thirdPlacePlayerName(first, second);
	
	//print out the "high score text"
	Rect high;
	high.bot = yres - 400;
	high.left = xres / 2 - 50;
	high.center = 0;
	ggprint16(&high, 0, 0xffffffff, "High Scores");
	
	// print out the first place score
	Rect best;
	best.bot = yres -450;
	best.left = xres/2 - 200;
	best.center = 0;
	const char* one = "1";
	ggprint16(&best, 0, 0xffffffff, one);
	best.left = xres/2;
	ggprint16(&best, 0, 0xffffffff, firstPlace.c_str());
	best.left = xres/2+ 200;
	std::string finalResult = std::to_string(first);
	ggprint16(&best, 0, 0xffffffff, finalResult.c_str());

	//print out the second place score
	Rect secondBest;
	secondBest.bot = yres - 500;
	secondBest.left = xres/2 - 200;
	secondBest.center = 0;
	const char* two = "2";
	ggprint16(&secondBest, 0, 0xffffffff, two);
	secondBest.left = xres/2;
	ggprint16(&secondBest, 0, 0xffffffff, secondPlace.c_str());
	secondBest.left = xres/2 + 200;
	std::string secondResult = std::to_string(second);
	ggprint16(&secondBest, 0, 0xffffffff, secondResult.c_str());

	//print out the third place score
	Rect thirdBest;
	thirdBest.bot = yres - 550;
	thirdBest.left = xres/2 - 200;
	thirdBest.center = 0;
	const char* three = "3";
	ggprint16(&thirdBest, 0, 0xffffffff, three);
	thirdBest.left = xres/2;
	ggprint16(&thirdBest, 0, 0xffffffff, thirdPlace.c_str());
	thirdBest.left = xres/2 + 200;
	std::string thirdResult = std::to_string(third);
	ggprint16(&thirdBest, 0, 0xffffffff, thirdResult.c_str());
}