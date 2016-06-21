/**
 * Include file for window/viewport options
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL2/SDL.h>


/*
 * Main window class
 */
class JWindow {
public:
	JWindow(int sx, int sy);
	~JWindow();
	int activate();
	int dispose();
	
	void update();
	
	
	bool isActive();
	
	int getScreenWidth();
	int getScreenHeight();

private:
	SDL_Window *window;
	std::string windowName;
	
	int screenWidth, screenHeight;
	bool needsRefresh, active;
	
	void refresh();
	void processInput();
	
};


#endif
