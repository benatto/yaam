all: udev.h filesystem.h signalhandler.h
	gcc -o yaamd main.c udev.c filesystem.c signalhandler.c `pkg-config --libs --cflags libudev` `pkg-config --libs --cflags blkid` -ggdb

clean:
	rm -fv yaamd *.o
