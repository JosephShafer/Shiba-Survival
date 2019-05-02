// Program: amberZ.h
// Author:  Amber Zaragoza
// Personal functions for Shiba Survival

#ifndef AMBERZ_H
#define AMBERZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <GL/glx.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "amberZ.h"
#include "Image.h"
#include "fonts.h"

#define PORT 443
#define USERAGENT "CMPS-3350"
#define MAX_READ_ERRORS 100

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

class SpriteTimer {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	SpriteTimer();
	double timeDiff(struct timespec *start, struct timespec *end);
	void timeCopy(struct timespec *dest, struct timespec *source);
	void recordTime(struct timespec *t);
};

extern Timer gameTimer;
extern SSD min1, min2, col, sec1, sec2;

void drawTimer(int);
void updateTimer(int, int);
void drawSprite(GLuint, Image&, float, float, float, float);
void updateFrame(Image&, SpriteTimer&, double);
void amberZ(int, int, GLuint);
BIO * ssl_setup_bio(void);
void set_non_blocking(const int sock);
void storeScore(char[], int);

#endif
