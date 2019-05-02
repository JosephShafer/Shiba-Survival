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

/*
//Usage:
//  ./lab3sslget <host> <page>

#define PORT 443
#define USERAGENT "CMPS-3350"
//---------------------------------------------------------------------
//Fixed warning: 'MAX_READ_ERRORS' does not name a type
//Previous code was:
//const MAX_READ_ERRORS = 100;
//---------------------------------------------------------------------
#define MAX_READ_ERRORS 100


int main(int argc, char *argv[])
{
    BIO *ssl_setup_bio(void);
    void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
    void set_non_blocking(const int sock);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;
    //-----------------------------------------------------------------------
    //Fixed warning: invalid conversion from 'const SSL_METHOD* to
    //               'SSL_METHOD*'
    //Previous code was:
    //SSL_METHOD *method;
    //-----------------------------------------------------------------------
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char req[1000];
    int req_len;
    char hostname[256] = "www.google.com";
    char pagename[256] = "";
    int port = PORT;
    int bytes, nreads, nerrs;
    char buf[256];
    int ret;

    //Get any command-line arguments.
    if (argc > 1)
        strcpy(hostname, argv[1]);
    if (argc > 2)
        strcpy(pagename, argv[2]);

    //Setup the SSL BIO
    outbio = ssl_setup_bio();

    //Initialize the SSL library
    if (SSL_library_init() < 0)
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);

    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n",
                   argv[0], hostname, inet_ntoa(addr.sin_addr), port);
    }

    //Connect using the SSL certificate.
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);

    //Show the certificate data
    show_cert_data(ssl, outbio, hostname);

    //A non-blocking socket will make the ssl_read() not block.
    set_non_blocking(sd);

    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);    
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);

    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n");
        fflush(stderr);
    }

    //Get data returned from the server.
    //First, do priming read.
    //We can take this approach because our socket is non-blocking.
    //Start with an error condition.
    bytes = -1;
    memset(buf, '\0', sizeof(buf));
    while (bytes <= 0) {
        bytes = SSL_read(ssl, buf, sizeof(buf));

        //A slight pause can cause fewer reads to be needed.
        usleep(10000);
    }

    //A successful priming read was accomplished.
    //Now read all the data.
    nreads = 1;

    //Allow for some read errors to happen, while getting the complete data.
    nerrs = 0;

    while (bytes >= 0 && nerrs < MAX_READ_ERRORS) {
        write(STDOUT_FILENO, buf, bytes);
        memset(buf, '\0', sizeof(buf));
        ++nreads;
        bytes = SSL_read(ssl, buf, sizeof(buf));
	
        if (bytes == 0)
            ++nerrs;
        else
            nerrs = 0;

        //A slight pause can cause fewer reads to be needed.
        usleep(20000);
    }
    printf("\nn calls to ssl_read(): %i\n", nreads);
    fflush(stdout);

    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);

    return 0;
}

BIO *ssl_setup_bio(void)
{
    //Setup the ssl BIO, basic I/O abstraction.
    //https://www.openssl.org/docs/man1.1.0/man3/bio.html
    BIO *bio = NULL;
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    bio = BIO_new(BIO_s_file());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);

    return bio;
}

void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname)
{
    //Display ssl certificate data here.
    //Get the remote certificate into the X509 structure
    printf("--------------------------------------------------------------\n");
    printf("Certificate data...\n");
    X509 *cert;
    X509_NAME *certname;
    printf("calling SSL_get_peer_certificate(ssl)\n");
    cert = SSL_get_peer_certificate(ssl);

    if (cert == NULL)
        printf("Error: Could not get a certificate from: %s.\n", hostname);
    else
        printf("Retrieved the server's certificate from: %s.\n", hostname);

    //extract various certificate information
    certname = X509_NAME_new();
    certname = X509_get_subject_name(cert);

    //display the cert subject here
    if (BIO_printf(outbio, "Displaying the certificate subject data:\n") < 0)
        fprintf(stderr, "ERROR: BIO_printf\n");
    X509_NAME_print_ex(outbio, certname, 0, 0);
    if (BIO_printf(outbio, "\n\n") < 0)
        fprintf(stderr, "ERROR: BIO_printf\n");
    printf("--------------------------------------------------------------\n");
}

void set_non_blocking(const int sock)
{
    //Set a socket to be non-blocking.
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
*/