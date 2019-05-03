/*
 Program: amberZ.cpp
 Author:  Amber Zaragoza
 Personal functions for Shiba Survival
**/

#include "amberZ.h"

/*
 SSD: Class used to render the seven-segment display game timer
**/
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

/*
 SSDTimer: Class used to update SSD timer
**/
void SSDTimer::startTimer()
{
	startTime = std::chrono::system_clock::now();
	isRunning = true;
}

void SSDTimer::stopTimer()
{
	endTime = std::chrono::system_clock::now();
	isRunning = false;
}

double SSDTimer::getElapsedMilliseconds()
{
	std::chrono::time_point<std::chrono::system_clock> endTime;
	if (isRunning) {
		endTime = std::chrono::system_clock::now();
	}
	else {
		endTime = endTime;
	}
	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}

double SSDTimer::getElapsedSeconds()
{
	return getElapsedMilliseconds() / 1000.0;
}

double SSDTimer::getElapsedMinutes()
{
	return getElapsedSeconds() / 60.0;
}

/*
 SpriteTimer: Class used to handle the sprite animation speed
**/
SpriteTimer::SpriteTimer() {
	physicsRate = 1.0 / 30.0;
	oobillion = 1.0 / 1e9;
}

double SpriteTimer::timeDiff(struct timespec *start, struct timespec *end) {
	return (double) (end->tv_sec - start->tv_sec ) +
		(double) (end->tv_nsec - start->tv_nsec) * oobillion;
}

void SpriteTimer::timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}

void SpriteTimer::recordTime(struct timespec *t) {
	clock_gettime(CLOCK_REALTIME, t);
}

AmbersGlobals *AmbersGlobals::instance = 0;
AmbersGlobals *ag = ag->getInstance();

/*
 drawTimer(): Function used to render the SSD timer
**/
void drawTimer(int xres)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslated((xres / 2) - 100, 595.0f, 0.0f);
		ag->minute1.renderSSD();
	glPopMatrix();
	glPushMatrix();
		glTranslated((xres / 2) - 50, 595.0f, 0.0f);
		ag->minute2.renderSSD();
	glPopMatrix();
	glPushMatrix();
		glTranslated((xres / 2), 595.0f, 0.0f);
		ag->colon.renderColon();
		ag->colon.updateDisplay(10);
	glPopMatrix();
	glPushMatrix();
		glTranslated((xres / 2) + 20, 595.0f, 0.0f);
		ag->second1.renderSSD();
	glPopMatrix();
	glPushMatrix();
		glTranslated((xres / 2) + 70, 595.0f, 0.0f);
		ag->second2.renderSSD();
	glPopMatrix();
}

/*
 updateTimer(): Function used to update the SSD timer
**/
void updateTimer(int min, int sec)
{
	if (min > 9) {
		ag->minute1.updateDisplay(min / 10);
		ag->minute2.updateDisplay(min % 10);
	}
	else {
		ag->minute1.updateDisplay(0);
		ag->minute2.updateDisplay(min);
	}
	if (sec > 9) {
		ag->second1.updateDisplay(sec / 10);
		ag->second2.updateDisplay(sec % 10);
	}
	else {
		ag->second1.updateDisplay(0);
		ag->second2.updateDisplay(sec);
	}
}

/*
 drawSprite(): Function used to render sprites
**/
void drawSprite(GLuint texture, Image &sprite, float width, float height, float xpos, float ypos)
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
	if (sprite.frame >= sprite.columns) {
		iy = 1;
	}
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

/*
 updateFrame(): Function used to animate sprites
**/
void updateFrame(Image &sprite, SpriteTimer &timer, double delay)
{
	timer.recordTime(&timer.currentTime);
	double timeSpan = timer.timeDiff(&timer.animationTime, &timer.currentTime);
	if (timeSpan > delay) {
		++sprite.frame;
		if (sprite.frame >= sprite.columns) {
			sprite.frame = 0;
		}
		timer.recordTime(&timer.animationTime);
	}
}

/*
 amberZ(): Function used to display my name and photo on the credits screen
**/
void amberZ(int x, int y, GLuint textureId)
{
	float wid = 120.0f;
	glColor3ub(255, 255, 255);
	glPushMatrix();
	glTranslatef((float) x, (float) y, 0);
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
	Rect r;
	r.bot = y;
	r.left = x + 90;
	r.center = 0;
	ggprint16(&r, 16, 0x00ffffff, "Amber Zaragoza");
}

BIO * ssl_setup_bio(void)
{
	BIO * bio = NULL;
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	bio = BIO_new(BIO_s_file());
	bio = BIO_new_fp(stdout, BIO_NOCLOSE);
	return bio;
}

void set_non_blocking(const int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts < 0) {
		perror("ERROR: fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock, F_SETFL, opts) < 0) {
		perror("ERROR: fcntl(O_NONBLOCK)");
		exit(EXIT_FAILURE);
	}
}

void storeScore(char user[], int score)
{
	int sd;
	struct hostent *host;
	struct sockaddr_in addr;
	BIO *outbio = NULL;
	const SSL_METHOD *method;
	SSL_CTX *ctx;
	SSL *ssl;
	char req[1000];
	int req_len;
	char hostname[256] = "cs.csubak.edu";
	char pagename[256];
	sprintf(pagename, "~azaragoza/3350/Shiba-Survival/save_scores.php?user=%s&score=%d", user, score);
	int port = ag->port;
	int bytes;
	char buf[256];
	int ret;
	outbio = ssl_setup_bio();
	if (SSL_library_init() < 0)
		BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
	method = SSLv23_client_method();
	ctx = SSL_CTX_new(method);
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
	host = gethostbyname(hostname);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n", hostname, hostname, inet_ntoa(addr.sin_addr), port);
	}
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sd);
	SSL_connect(ssl);
	set_non_blocking(sd);
	sprintf(req, "GET /%s HTTP/1.1\r\nUser-Agent: %s\r\nHost: %s\r\n\r\n", pagename, hostname, ag->userAgent);
	//printf("%s\n", req);
	req_len = strlen(req);
	ret = SSL_write(ssl, req, req_len);
	if (ret <= 0) {
		fprintf(stderr, "ERROR: SSL_write\n");
		fflush(stderr);
	}
	bytes = -1;
	memset(buf, '\0', sizeof(buf));
	while (bytes <= 0) {
		bytes = SSL_read(ssl, buf, sizeof(buf));
		usleep(10000);
	}
	fflush(stdout);
	SSL_free(ssl);
	close(sd);
	SSL_CTX_free(ctx);
}