OBJS = AnalogClock.c

OBJ_NAME = AnalogClock

all : $(OBJS)
	gcc $(OBJS) -lm -Wall -ansi -pedantic -mwindows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)
