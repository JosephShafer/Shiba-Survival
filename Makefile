COMPILER = g++
CFLAGS   = -I ./include
FILES    = shiba.cpp log.cpp timers.cpp amberZ.cpp josephS.cpp danL.cpp mabelleC.cpp thomasB.cpp Image.cpp
FONTS    = libggfonts.a
LFLAGS   = -lrt -lX11 -lGLU -lGL -pthread -lm -lcrypto -lssl

all: shiba debug

shiba: $(FILES)
	$(COMPILER) $(CFLAGS) $(FILES) $(FONTS) -Wall -Wextra $(LFLAGS) -oshiba

debug: $(FILES)
	$(COMPILER) $(CFLAGS) $(FILES) $(FONTS) -Wall -Wextra $(LFLAGS) -DDEBUG -odebug

clean:
	rm -f shiba debug *.o
