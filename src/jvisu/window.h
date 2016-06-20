/**
 * Include file for window/viewport options
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>


/*
 * Main window class
 */
class JWindow {
public:
	JWindow(int sx, int sy);
	~JWindow();
	
	void update();
	
	int getScreenWidth();
	int getScreenHeight();

private:
	SDL_Window *window;
	int screenWidth, screenHeight;
};


#endif
