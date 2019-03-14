// Program: amberZ.cpp
// Author:  Amber Zaragoza
// Personal functions for Shiba Survival

#include <stdio.h>
#include <curl/curl.h>
#include <GL/glx.h>
#include "fonts.h"

void amberZ(int x, int y, GLuint textid)
{
	// Show picture
	float wid = 120.0f;
	glColor3ub(255, 255, 255);
	glPushMatrix();
	glTranslatef((float) x, (float) y, 0);
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
	ggprint16(&r, 16, 0x00ffff00, "Amber Zaragoza");
}

void gameOverScreen()
{
	//
}

void storeScore(char user[], int score)
{
	//send score data over to server
	//will be displayed on the score page
	//use "sudo apt install libcurl4-openssl-dev" for library
	char post_data[256];
	snprintf(post_data, sizeof(post_data), "user=%s&score=%d", user, score);
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		// Set the URL that receives POST
		curl_easy_setopt(curl, CURLOPT_URL, "https://cs.csubak.edu/~azaragoza/3350/Shiba-Survival/save_scores.php");
		// Set POST data
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}