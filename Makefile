all: udev.h filesystem.h
	gcc -o yaamd main.c udev.c filesystem.c `pkg-config --libs --cflags libudev` -ggdb

clean:
	rm -fv yaamd *.o
