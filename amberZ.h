// Program: amberZ.h
// Author:  Amber Zaragoza
// Personal functions for Shiba Survival

#ifndef AMBERZ_H
#define AMBERZ_H

#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
//#include <curl/curl.h>
#include <GL/glx.h>
#include "fonts.h"

class Timer
{
  private:
	std::chrono::time_point<std::chrono::system_clock> startTime;
	std::chrono::time_point<std::chrono::system_clock> endTime;
	bool isRunning = false;
  public:
	void startTimer();
	void stopTimer();
	double getElapsedMilliseconds();
	double getElapsedSeconds();
	double getElapsedMinutes();
};

class SSD
{
	private:
		std::bitset<7> screen;
	public:
		SSD();
		void updateDisplay(int);
		void renderSSD();
		void renderColon();
};

extern Timer gameTimer;
extern SSD min1, min2, col, sec1, sec2;

void drawTimer(int);
void updateTimer(int, int);
void amberZ(int, int, GLuint);
void storeScore(char[], int);

#endif
