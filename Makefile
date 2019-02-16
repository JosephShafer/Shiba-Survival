COMPILER = g++
CFLAGS   = -I ./include
FILES    = asteroids.cpp log.cpp timers.cpp amberZ.cpp josephS.cpp danL.cpp mabelleC.cpp
FONTS    = libggfonts.a
LFLAGS   = -lrt -lX11 -lGLU -lGL -pthread -lm

all: asteroids

asteroids: $(FILES)
	$(COMPILER) $(CFLAGS) $(FILES) $(FONTS) -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o
