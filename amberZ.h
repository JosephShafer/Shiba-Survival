/*
 Program: amberZ.h
 Author:  Amber Zaragoza
 Personal functions for Shiba Survival
**/

#ifndef AMBERZ_H
#define AMBERZ_H

#include <bitset>
#include <chrono>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h> 
#include <vector>
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

class SSDTimer
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

class SpriteTimer {
	public:
		double physicsRate;
		double oobillion;
		struct timespec startTime;
		struct timespec endTime;
		struct timespec currentTime;
		struct timespec animationTime;
		SpriteTimer();
		double timeDiff(struct timespec* start, struct timespec* end);
		void timeCopy(struct timespec* dest, struct timespec* source);
		void recordTime(struct timespec* t);
};

class AmbersGlobals {
	public:
		int xres;
		int yres;
		SSD minute1;
		SSD minute2;
		SSD colon;
		SSD second1;
		SSD second2;
		SSDTimer gameTimer;
		int port;
		char *userAgent;
		int maxReadErrors;
		int topScores;
		std::vector<std::pair<std::string, int>> scores;
		static AmbersGlobals *instance;
		static AmbersGlobals *getInstance() {
			if (!instance) {
				instance = new AmbersGlobals;
			}
			return instance;
		}
		AmbersGlobals() {
			xres = 1366;
			yres = 766;
			port = 443;
			userAgent = (char *) "CMPS-3350";
			maxReadErrors = 100;
			topScores = 1;
		}
};

void drawTimer(int);
void updateTimer(int, int);
void drawSprite(GLuint, Image&, float, float, float, float);
void updateFrame(Image&, SpriteTimer&, double);
void amberZ(int, int, GLuint);
BIO *sslSetupBIO(void);
void setNonBlocking(const int);
void connectToWebsite(char[], char[]);
bool sortbysec(const std::pair<std::string, int>&, const std::pair<std::string, int>&);
void storeScore(char[], int);
void getTopScores();
int getRanking(std::string, int);
void showScores();

#endif