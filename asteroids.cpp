//Program: asteroids.cpp
//Author:  Gordon Griesel
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
#include "log.h"
#include "fonts.h"
#include "josephS.h"
#include "amberZ.h"
#include "thomasB.h"

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
	// TODO This could be an array or vector
	bool gameMenu = true;
	bool gameStart = false;
	
	GLuint textures[5];
	/*
	GLuint amberZTexture;
	GLuint danLTexture;
	GLuint josephSTexture;
	GLuint mabelleCTexture;
	GLuint thomasBTexture;
	*/
	static Global *instance;
	static Global *getInstance() {
		if (!instance) {
			instance = new Global;
		}
		return instance;
	}
	Global() {
		xres = 1250;
		yres = 900;
		memset(keys, 0, 65536);
		showCredits = false;
	}
};
Global *Global::instance = 0;
Global *gl = gl->getInstance();

//Added image class from rainforest.cpp
//TODO We could probably name this something like Texture and put
// it in another file
class Image {
public:
	int width, height;
	unsigned char *data;
	const char *file;
	~Image() { delete [] data; }
	Image(const char *fname) {
		file = fname;
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		// TODO this logic converts images to ppm format, probably can be removed
		int ppmFlag = 0; 
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};

Image img[5] = {
"./images/amberZ.png",
"./images/josephS.png",
"./images/danL.png",
"./images/mabelleC.png",
"./images/thomasB.png"};

// TODO rename to dog?
//dog
class Ship {
public:
	Vec dir;
	Vec pos;
	Vec vel;
	// TODO How we 'display' angle will depend on how the sprites look
	float angle;
	// TODO won't need color if we're doing sprites..
	float color[3];
public:
	Ship() {
		VecZero(dir);
		pos[0] = (Flt)(gl->xres/2);
		pos[1] = (Flt)(gl->yres/2);
		pos[2] = 0.0f;
		VecZero(vel);
		angle = 0.0;
		color[0] = color[1] = color[2] = 1.0;
	}
};

// TODO Do we want to have bullets at all?
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

// TODO Maybe rename this to Enemy? 
/*
	Different enemies:
	- Cats
	- Mailmen
	Powerups
*/
/*
class Asteroid {
public:
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
public:
	Asteroid() {
		prev = NULL;
		next = NULL;
	}
};
*/

/*
class Enemy {
public:
	Vec pos;
	Vec vel;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
public:
};
*/

//sets up game state
class Game {
public:
	Ship ship;
	//Asteroid *ahead;
	Bullet *barr;
	//int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
public:
	Game() {
		//ahead = NULL;
		barr = new Bullet[MAX_BULLETS];
		//nasteroids = 0;
		nbullets = 0;
		mouseThrustOn = false;
		//build 10 asteroids...
		// for (int j=0; j<10; j++) {
		// 	Asteroid *a = new Asteroid;
		// 	a->nverts = 8;
		// 	a->radius = rnd()*80.0 + 40.0;
		// 	Flt r2 = a->radius / 2.0;
		// 	Flt angle = 0.0f;
		// 	Flt inc = (PI * 2.0) / (Flt)a->nverts;
		// 	for (int i=0; i<a->nverts; i++) {
		// 		a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
		// 		a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
		// 		angle += inc;
		// 	}
		// 	a->pos[0] = (Flt)(rand() % gl->xres);
		// 	a->pos[1] = (Flt)(rand() % gl->yres);
		// 	a->pos[2] = 0.0f;
		// 	a->angle = 0.0;
		// 	a->rotate = rnd() * 4.0 - 2.0;
		// 	a->color[0] = 0.8;
		// 	a->color[1] = 0.8;
		// 	a->color[2] = 0.7;
		// 	a->vel[0] = (Flt)(rnd()*2.0-1.0);
		// 	a->vel[1] = (Flt)(rnd()*2.0-1.0);
		// 	//std::cout << "asteroid" << std::endl;
		// 	//add to front of linked list
		// 	a->next = ahead;
		// 	if (ahead != NULL)
		// 		ahead->prev = a;
		// 	ahead = a;
		// 	++nasteroids;
		// }
		clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	~Game() {
		delete [] barr;
	}
} g;

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
} x11(0, 0);

//function prototypes
void init_opengl(void);
int check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void physicsKeyEvents();
void shipControl();
void bulletPositionControl();
void shootBullet();
void render();
void gameplayScreen();
void drawBullet();
void drawShip();
void drawCredits();
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

	for (int i = 0; i < 5; i++) {
		glGenTextures(1, &gl->textures[i]);
		glBindTexture(GL_TEXTURE_2D, gl->textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, img[i].width, img[i].height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, img[i].data);
	}

	/*
	//create opengl texture elements
	// TODO gotta be a way to make this simpler
	glGenTextures(1, &gl->amberZTexture);
	glBindTexture(GL_TEXTURE_2D, gl->amberZTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[0].height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, img[0].data);

	glGenTextures(1, &gl->danLTexture);
	glBindTexture(GL_TEXTURE_2D, gl->danLTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[2].height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, img[2].data);

	glGenTextures(1, &gl->josephSTexture);
	glBindTexture(GL_TEXTURE_2D, gl->josephSTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[1].width, img[1].height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, img[1].data);
	
	glGenTextures(1, &gl->mabelleCTexture);
	glBindTexture(GL_TEXTURE_2D, gl->mabelleCTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[3].height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, img[3].data);
	
	
	glGenTextures(1, &gl->thomasBTexture);
	glBindTexture(GL_TEXTURE_2D, gl->thomasBTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[4].height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, img[4].data);
	*/
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

// TODO Do we want mouse support?
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
							printf("High score was clicked\n");;
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
/*
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			//a little time between each bullet
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&g.bulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {
					Bullet *b = &g.barr[g.nbullets];
					timeCopy(&b->time, &bt);
					b->pos[0] = g.ship.pos[0];
					b->pos[1] = g.ship.pos[1];
					b->vel[0] = g.ship.vel[0];
					b->vel[1] = g.ship.vel[1];
					//convert ship angle to radians
					Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
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
					++g.nbullets;
				}
			}
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	//keys[XK_Up] = 0;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		int xdiff = savex - e->xbutton.x;
		int ydiff = savey - e->xbutton.y;
		if (++ct < 10)
			return;		
		//std::cout << "savex: " << savex << std::endl << std::flush;
		//std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
		//std::flush;
		if (xdiff > 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.ship.angle += 0.05f * (float)xdiff;
			if (g.ship.angle >= 360.0f)
				g.ship.angle -= 360.0f;
		}
		else if (xdiff < 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.ship.angle += 0.05f * (float)xdiff;
			if (g.ship.angle < 0.0f)
				g.ship.angle += 360.0f;
		}
		if (ydiff > 0) {
			//apply thrust
			//convert ship angle to radians
			Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			g.ship.vel[0] += xdir * (float)ydiff * 0.01f;
			g.ship.vel[1] += ydir * (float)ydiff * 0.01f;
			Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
					g.ship.vel[1]*g.ship.vel[1]);
			if (speed > 10.0f) {
				speed = 10.0f;
				normalize2d(g.ship.vel);
				g.ship.vel[0] *= speed;
				g.ship.vel[1] *= speed;
			}
			g.mouseThrustOn = true;
			clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
		}
		x11.set_mouse_position(100,100);
		savex=100;
		savey=100;
	}
}
*/

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
			if (gl-> showCredits){
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
			createEnemy(i);
			#ifdef joeydebug
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

/*
void deleteAsteroid(Game *g, Asteroid *node)
{
	//Remove a node from doubly-linked list
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ahead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ahead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}
*/

/*
void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0;
	Flt r2 = ta->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
	ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0;
	ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
	ta->color[0] = 0.8;
	ta->color[1] = 0.8;
	ta->color[2] = 0.7;
	ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
	ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
	//std::cout << "frag" << std::endl;
}
*/

void physics()
{
	
	//
	//
	//
	shipControl();
	//Update bullet positions
	bulletPositionControl();
	//
	//Update asteroid positions
	/*
	Asteroid *a = g.ahead;
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -100.0) {
			a->pos[0] += (float)gl->xres+200;
		}
		else if (a->pos[0] > (float)gl->xres+100) {
			a->pos[0] -= (float)gl->xres+200;
		}
		else if (a->pos[1] < -100.0) {
			a->pos[1] += (float)gl->yres+200;
		}
		else if (a->pos[1] > (float)gl->yres+100) {
			a->pos[1] -= (float)gl->yres+200;
		}
		a->angle += a->rotate;
		a = a->next;
	}
	*/
	//
	//Asteroid collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break the asteroid into pieces
	//        if asteroid small, delete it
	/*
	a = g.ahead;
	while (a) {
		//is there a bullet within its radius?
		int i=0;
		while (i < g.nbullets) {
			Bullet *b = &g.barr[i];
			d0 = b->pos[0] - a->pos[0];
			d1 = b->pos[1] - a->pos[1];
			dist = (d0*d0 + d1*d1);
			if (dist < (a->radius*a->radius)) {
				//std::cout << "asteroid hit." << std::endl;
				//this asteroid is hit.
				if (a->radius > MINIMUM_ASTEROID_SIZE) {
					//break it into pieces.
					Asteroid *ta = a;
					buildAsteroidFragment(ta, a);
					int r = rand()%10+5;
					for (int k=0; k<r; k++) {
						//get the next asteroid position in the array
						Asteroid *ta = new Asteroid;
						buildAsteroidFragment(ta, a);
						//add to front of asteroid linked list
						ta->next = g.ahead;
						if (g.ahead != NULL)
							g.ahead->prev = ta;
						g.ahead = ta;
						g.nasteroids++;
					}
				} else {
					a->color[0] = 1.0;
					a->color[1] = 0.1;
					a->color[2] = 0.1;
					//asteroid is too small to break up
					//delete the asteroid and bullet
					Asteroid *savea = a->next;
					deleteAsteroid(&g, a);
					a = savea;
					g.nasteroids--;
				}
				//delete the bullet...
				memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
				g.nbullets--;
				if (a == NULL)
					break;
			}
			i++;
		}
		if (a == NULL)
			break;
		a = a->next;
	}
	*/
	//---------------------------------------------------
	//check keys pressed now
	physicsKeyEvents();
}

//Also movement stuff in Check Keys
void shipControl()
{
	//Flt d0,d1,dist;
	//Update ship position
	g.ship.pos[0] += g.ship.vel[0];
	g.ship.pos[1] += g.ship.vel[1];
	//Check for collision with window edges
	if (g.ship.pos[0] < 0.0) {
		g.ship.pos[0] += (float)gl->xres;
	}
	else if (g.ship.pos[0] > (float)gl->xres) {
		g.ship.pos[0] -= (float)gl->xres;
	}
	else if (g.ship.pos[1] < 0.0) {
		g.ship.pos[1] += (float)gl->yres;
	}
	else if (g.ship.pos[1] > (float)gl->yres) {
		g.ship.pos[1] -= (float)gl->yres;
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
		i++;
	}
}

// Don't want to confuse for checkKeys, could we combine those?
void physicsKeyEvents()
{
	if (gl->keys[XK_Left]) {
		/*
		g.ship.angle += 4.0;
		if (g.ship.angle >= 360.0f)
			g.ship.angle -= 360.0f;
		*/
		g.ship.angle = 90;
		g.ship.pos[0] -= 5;
	}
	if (gl->keys[XK_Right]) {
		/*
		g.ship.angle -= 4.0;
		if (g.ship.angle < 0.0f)
			g.ship.angle += 360.0f;
		*/
		g.ship.angle = 270;
		g.ship.pos[0] += 5;
	}
	if (gl->keys[XK_Up]) {
		/*
		//apply thrust
		//convert ship angle to radians
		Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		g.ship.vel[0] += xdir*0.02f;
		g.ship.vel[1] += ydir*0.02f;
		Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
				g.ship.vel[1]*g.ship.vel[1]);
		if (speed > 10.0f) {
			speed = 10.0f;
			normalize2d(g.ship.vel);
			g.ship.vel[0] *= speed;
			g.ship.vel[1] *= speed;
		}
		*/
		g.ship.angle = 360;
		g.ship.pos[1] += 5;
	}
	if (gl->keys[XK_Down]) {
		g.ship.pos[1] -= 5;
		g.ship.angle = 180;
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
			b->pos[0] = g.ship.pos[0];
			b->pos[1] = g.ship.pos[1];
			b->vel[0] = g.ship.vel[0];
			b->vel[1] = g.ship.vel[1];
			//convert ship angle to radians
			Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
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
	//Draw the ship
	drawShip();
	//-------------------------------------------------------------------------
	//Draw the asteroids
	/**
	{
		Asteroid *a = g.ahead;
		while (a) {
			//Log("draw asteroid...\n");
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
			//Log("%i verts\n",a->nverts);
			for (int j=0; j<a->nverts; j++) {
				glVertex2f(a->vert[j][0], a->vert[j][1]);
			}
			glEnd();
			//glBegin(GL_LINES);
			//	glVertex2f(0,   0);
			//	glVertex2f(a->radius, 0);
			//glEnd();
			glPopMatrix();
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(a->pos[0], a->pos[1]);
			glEnd();
			a = a->next;
		}
	}
	**/
	//-------------------------------------------------------------------------
	//Draw the bullets
	drawBullet();
	drawTimer(gl->xres);
	//createEnemy(1);
	updateAllPosition(g.ship.pos[0], g.ship.pos[1], gl->xres, gl->yres);


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

void drawShip()
{
	glColor3fv(g.ship.color);
	glPushMatrix();
	glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
	//float angle = atan2(ship.dir[1], ship.dir[0]);
	glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	//glVertex2f(-10.0f, -10.0f);
	//glVertex2f(  0.0f, 20.0f);
	//glVertex2f( 10.0f, -10.0f);
	glVertex2f(-12.0f, -10.0f);
	glVertex2f(  0.0f, 20.0f);
	glVertex2f(  0.0f, -6.0f);
	glVertex2f(  0.0f, -6.0f);
	glVertex2f(  0.0f, 20.0f);
	glVertex2f( 12.0f, -10.0f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
	if (gl->keys[XK_Up] || gl->keys[XK_Down] || gl->keys[XK_Right] || gl->keys[XK_Left] || g.mouseThrustOn) {
		int i;
		//draw thrust
		Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		Flt xs,ys,xe,ye,r;
		glBegin(GL_LINES);
		for (i=0; i<16; i++) {
			xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
			ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
			r = rnd()*40.0+40.0;
			xe = -xdir * r + rnd() * 18.0 - 9.0;
			ye = -ydir * r + rnd() * 18.0 - 9.0;
			glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
			glVertex2f(g.ship.pos[0]+xs,g.ship.pos[1]+ys);
			glVertex2f(g.ship.pos[0]+xe,g.ship.pos[1]+ye);
		}
		glEnd();
	}
}

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