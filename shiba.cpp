//Program: shiba.cpp
//Authors: Amber Zaragoza, Joseph Shafer, Mabelle Cruz, Thomas Basden, Dan Leinker
//Framework: asteroids.cpp
//Framework Author: Gordon Griesel
//Date:    2014 - 2018
//Mod Spring 2015: Added constructors
//This program is a game starting point for a 3350 project.

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl->h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <vector>
#include "amberZ.h"
#include "josephS.h"
#include "thomasB.h"
#include "Image.h"
#include "fonts.h"
#include "log.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

class Global {
public:
	int xres, yres;
	char keys[65536];
	bool showCredits;
	bool gameMenu = true;
	bool gameStart = false;
	float score;
	
	GLuint textures[6];
	static Global *instance;
	static Global *getInstance() {
		if (!instance) {
			instance = new Global;
		}
		return instance;
	}
	Global() {
		xres = 1366;
		yres = 768;
		memset(keys, 0, 65536);
		showCredits = false;
		score = 0;
	}
};
Global *Global::instance = 0;
Global *gl = gl->getInstance();

//dog
class Shiba {
public:
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
public:
	Shiba() {
		VecZero(dir);
		pos[0] = (Flt)(gl->xres/2);
		pos[1] = (Flt)(gl->yres/2);
		pos[2] = 0.0f;
		VecZero(vel);
		angle = 0.0;
		color[0] = color[1] = color[2] = 1.0;
	}
};

//keep?
class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
public:
	Bullet() { }
};

/*
	Different enemies:
	- Cats
	- Mailmen
	Powerups
*/

//sets up game state
class Game {
public:
	Shiba shiba;
	Bullet *barr;
	int nbullets;
	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
public:
	Game() {
		
		barr = new Bullet[MAX_BULLETS];
		nbullets = 0;
		mouseThrustOn = false;
		clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	~Game() {
		delete [] barr;
	}
} g;

Image img[6] = {
	Image("./images/amberZ.png"),
	Image("./images/josephS.png"),
	Image("./images/danL.png"),
	Image("./images/mabelleC.png"),
	Image("./images/thomasB.png"),
	Image("./images/Shiba-Sprites.png", 9, 4)
};

//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(gl->xres, gl->yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			std::cout << "\n\tcannot connect to X server" << std::endl;
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen=0;
		gl->xres = w;
		gl->yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			gl->xres = getWinAttr.width;
			gl->yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			XGrabKeyboard(dpy, root, False,
				GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			std::cout << "\n\tno appropriate visual found\n" << std::endl;
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
			StructureNotifyMask | SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			winops |= CWOverrideRedirect;
			swa.override_redirect = True;
		}
		//win = XCreateWindow(dpy, root, 0, 0, gl->xres, gl->yres, 0,
		//	vi->depth, InputOutput, vi->visual, winops, &swa);
		win = XCreateWindow(dpy, root, 0, 0, gl->xres, gl->yres, 0,
			vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Shiba Survival");
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != gl->xres || xce.height != gl->yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl->xres, 0, gl->yres, -1, 1);
		set_title();
	}
	void setup_screen_res(const int w, const int h) {
		gl->xres = w;
		gl->yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	//bullet stuff?
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			std::cout << "error: out of memory." << std::endl;
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes you the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(gl->xres, gl->yres);

//function prototypes
void init_opengl(void);
int check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void physicsKeyEvents();
void shibaControl();
void bulletPositionControl();
void shootBullet();
void render();
void gameplayScreen();
void drawBullet();
void drawCredits();
void updateFrame();
extern void menu();
extern int nbuttons;
extern Button button[];
//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	init_opengl();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
	int done = 0;

	enemyGetResolution(gl->xres, gl->yres);
	
	while (!done) {
		if(gl->gameStart != 1){
			gameTimer.startTimer();	
		}
		//update timer
		updateTimer((int) gameTimer.getElapsedMinutes(), ((int) gameTimer.getElapsedSeconds() % 60));
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics();
			physicsCountdown -= physicsRate;
		}
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl->xres, gl->yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl->xres, 0, gl->yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	for (int i = 0; i < 6; i++) {
		glGenTextures(1, &gl->textures[i]);
		glBindTexture(GL_TEXTURE_2D, gl->textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, img[i].width, img[i].height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, img[i].data);
	}
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

int check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return 0;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = gl->yres - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i < nbuttons; i++) {
		button[i].over=0;
		if (x >= button[i].r.left &&
			x <= button[i].r.right &&
			y >= button[i].r.bot &&
			y <= button[i].r.top) {
			button[i].over=1;
			if (button[i].over) {
				if (lbutton) {
					switch(i) {
						case 0:
							gl->gameMenu ^= 1;
							gl->gameStart ^= 1;
							printf("Resume was clicked!\n");
							break;
						case 1:
							printf("How to play was clicked\n");
							break;
						case 2:
							printf("High score was clicked\n");
							break;
						case 3:
							printf("Credits was clicked\n");
							gl->showCredits ^= 1;
							break;
						case 4:
							printf("Quit was clicked\n");
							exit(0);
					}
				}
			}
		}
	}
	return 0;
}

int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl->keys[key]=0;
		/*
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
		*/
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl->keys[key]=1;
		/*
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return 0;
		}
		*/
	} else {
		return 0;
	}
	if (shift){}
	switch (key) {
		static int i = 0;
		case XK_c:
			gl->showCredits ^= 1;
			break;
		case XK_Escape:
			if (gl->showCredits){
				gl->showCredits ^= 1;

				//should be on game over
				//left here for now until we add more functionality
				//storeScore(gl->user, gl->score);
				//return 1;
			}
			else {
				cleanupEnemies();
				gl->gameMenu ^= 1;
				gl->gameStart ^= 1;
			}
		
		case XK_f:
			break;
		case XK_s:
			i++;
			break;
		case XK_Down:
			break;
		case XK_equal:
			createEnemy(i, g.shiba.pos[0], g.shiba.pos[1]);
			#ifdef debug
			//printf("%d\n", int(enemies.size()));
			#endif
			break;
		case XK_minus:
			//can destroy enemy by index, right now is 0
			//will kill the first in a vector element
			destroyEnemy(0);
			break;
	}
	return 0;
}



void physics()
{
	shibaControl();
	//Update bullet positions
	bulletPositionControl();
	//check keys pressed now
	physicsKeyEvents();
}

//Also movement stuff in Check Keys
//This function also flips the shiba to other side if they 
//go over the edge
void shibaControl()
{
	//Flt d0,d1,dist;
	//Update shiba position
	g.shiba.pos[0] += g.shiba.vel[0];
	g.shiba.pos[1] += g.shiba.vel[1];
	//Check for collision with window edges
	if (g.shiba.pos[0] < 0.0) {
		g.shiba.pos[0] += (float)gl->xres;
	}
	else if (g.shiba.pos[0] > (float)gl->xres) {
		g.shiba.pos[0] -= (float)gl->xres;
	}
	else if (g.shiba.pos[1] < 0.0) {
		g.shiba.pos[1] += (float)gl->yres;
	}
	else if (g.shiba.pos[1] > (float)gl->yres) {
		g.shiba.pos[1] -= (float)gl->yres;
	}
}

void bulletPositionControl()
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
				sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)gl->xres;
		}
		else if (b->pos[0] > (float)gl->xres) {
			b->pos[0] -= (float)gl->xres;
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)gl->yres;
		}
		else if (b->pos[1] > (float)gl->yres) {
			b->pos[1] -= (float)gl->yres;
		}

		
		if(bulletHitEnemy(b->pos[0], b->pos[1], &gl->score)){
			//removes bullet if hit
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
				sizeof(Bullet));
			g.nbullets--;
		}
		
		i++;
	}
}

// Don't want to confuse for checkKeys, could we combine those?
void physicsKeyEvents()
{
	if (gl->keys[XK_Left]) {
		g.shiba.angle = 90;
		img[5].animation = 3;
		updateFrame();
		g.shiba.pos[0] -= 5;
	}
	if (gl->keys[XK_Right]) {
		g.shiba.angle = 270;
		img[5].animation = 1;
		updateFrame();
		g.shiba.pos[0] += 5;
	}
	if (gl->keys[XK_Up]) {
		g.shiba.angle = 360;
		img[5].animation = 2;
		updateFrame();
		g.shiba.pos[1] += 5;
	}
	if (gl->keys[XK_Down]) {
		g.shiba.angle = 180;
		img[5].animation = 0;
		updateFrame();
		g.shiba.pos[1] -= 5;
	}
	if (gl->keys[XK_space]) {
		shootBullet();
	}
	if (g.mouseThrustOn) {
		//should thrust be turned off
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
		//std::cout << "tdif: " << tdif << std::endl;
		if (tdif < -0.3)
			g.mouseThrustOn = false;
	}

}

// This creates the actual bullet, drawBullet() renders it, bulletPositionControl() makes it move. Could they be combined?
void shootBullet()
{
	//a little time between each bullet
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if (ts > 0.1) {
		timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < MAX_BULLETS) {
			//shoot a bullet...
			//Bullet *b = new Bullet;
			Bullet *b = &g.barr[g.nbullets];
			timeCopy(&b->time, &bt);
			b->pos[0] = g.shiba.pos[0];
			b->pos[1] = g.shiba.pos[1];
			b->vel[0] = g.shiba.vel[0];
			b->vel[1] = g.shiba.vel[1];
			//convert shiba angle to radians
			Flt rad = ((g.shiba.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			b->pos[0] += xdir*20.0f;
			b->pos[1] += ydir*20.0f;
			b->vel[0] += xdir*6.0f + rnd()*0.1;
			b->vel[1] += ydir*6.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
		}
	}
}

void render()
{
	//gameplayScreen();
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (gl->gameMenu){
		x11.show_mouse_cursor(1);
		menu();
	}
	if (gl->gameStart){
		x11.show_mouse_cursor(0);
		gameplayScreen();
		renderEnemies();
	}
	if (gl->showCredits) {
		drawCredits();
	}
}

void gameplayScreen()
{
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	//
	r.bot = gl->yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
	ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
	//ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
	//-------------------------------------------------------------------------
	//Draw the shiba
	//drawshiba();
	drawSprite(gl->textures[5], img[5], 40.0, 40.0, g.shiba.pos[0], g.shiba.pos[1]);

	//Draw the bullets
	drawBullet();
	drawTimer(gl->xres);
	textScoreDisplay(gl->score);
	//createEnemy(1);
	updateAllPosition(g.shiba.pos[0], g.shiba.pos[1]);

	primeSpawner(int(gameTimer.getElapsedMilliseconds()), g.shiba.pos[0], g.shiba.pos[1]);
}

void drawBullet()
{
	for (int i=0; i<g.nbullets; i++) {
		Bullet *b = &g.barr[i];
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
	}
}

int counter = 0;
void updateFrame() {
	++counter %= 5;
	printf ("%d\n", counter);
	if (counter == 4) {
		++img[5].frame;
	}
	if (img[5].frame >= 4)
		img[5].frame = 0;
}

/*
void drawshiba()
{
	float cx = g.shiba.pos[0];
	float cy = g.shiba.pos[1];
	float w = 40.0;
	float h = 40.0;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gl->textures[5]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = img[5].frame % img[5].columns;
	int iy = img[5].animation;
	if (img[5].frame >= img[5].columns)
		iy = 1;
	float tx = (float) ix / img[5].columns;
	float ty = (float) iy / img[5].rows;
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(tx, ty + (1.0 / img[5].rows));
	glVertex2i(cx - w, cy - h);
	glTexCoord2f(tx, ty);
	glVertex2i(cx - w, cy + h);
	glTexCoord2f(tx + (1.0 / img[5].columns), ty);
	glVertex2i(cx + w, cy + h);
	glTexCoord2f(tx + (1.0 / img[5].columns), ty + (1.0 / img[5].rows));
	glVertex2i(cx + w, cy - h);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
*/

void drawCredits()
{
		extern void amberZ(int, int, GLuint);
		extern void josephS(float, float, GLuint);
    extern void danL(int, int, GLuint);
    extern void mabelleC(int, int, GLuint);
		extern void thomasB(int, int, GLuint);
		glClear(GL_COLOR_BUFFER_BIT);
		Rect rcred;
		rcred.bot = gl->yres * 0.95f;
		rcred.left = gl->xres/2;
		rcred.center = 0;
		ggprint16(&rcred, 16, 0x00ffff00, "Credits");

		// moves pictures so they scale to monitors resolution
		float offset = 0.18f;
		amberZ((gl->xres/2 - 300), gl->yres * (1 - offset), gl->textures[0]);
		josephS((gl->xres/2 - 300), gl->yres * (1 - offset*2), gl->textures[1]);
		danL((gl->xres/2 - 300), gl->yres * (1 - offset*3), gl->textures[2]);
		mabelleC((gl->xres/2 - 300), gl->yres * (1 - offset*4), gl->textures[3]);
		thomasB((gl->xres/2 - 300), gl->yres * (1 - offset*5), gl->textures[4]);
}