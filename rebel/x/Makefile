all: game

CC = gcc
XLIB = -L/usr/X11R6/lib -I/usr/X11R6/include
LIBS = -lm -lX11
OBJS = rdefender.o
DEPENDS =
#CFLAGS = -g -Wall -pedantic -ansi
CFLAGS = -ansi
#OPTIONS = -D_GNU_SOURCE -DGAME_LINUX_X11 -DGAME_DEBUG
OPTIONS= -DGAME_LINUX_X11

.c.o: $(DEPENDS)
	$(CC) -c $(CFLAGS) $(OPTIONS) $< -o $@

game: $(OBJS) $(DEPENDS)
	$(CC) -o rdefender $(OBJS) $(XLIB) $(LIBS) $(OPTIONS)

clean:
	rm *.o

