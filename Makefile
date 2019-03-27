COMPILER = g++
CFLAGS   = -I ./include
FILES    = shiba.cpp log.cpp timers.cpp amberZ.cpp josephS.cpp danL.cpp mabelleC.cpp thomasB.cpp
FONTS    = libggfonts.a
LFLAGS   = -lrt -lX11 -lGLU -lGL -pthread -lm

all: shiba

shiba: $(FILES)
	$(COMPILER) $(CFLAGS) $(FILES) $(FONTS) -Wall -Wextra $(LFLAGS) -oshiba

joeydebug: $(FILES)
	$(COMPILER) $(CFLAGS) $(FILES) $(FONTS) -Wall -Wextra $(LFLAGS) -ojoeydebug -Djoeydebug

clean:
	rm -f shiba joeydebug *.o
