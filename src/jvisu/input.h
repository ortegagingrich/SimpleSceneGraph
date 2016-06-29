/*
 * Definitions and Declarations of input event objects
 */
#ifndef INPUT_H
#define INPUT_H

#include "sdl.h"

class JWindow;


class InputEvent {
public:
	// Factory Method
	static InputEvent *createInputEvent(SDL_Event event, JWindow *win);
	
	const JWindow *window;
	SDL_Event sdlEvent;
	
	InputEvent(SDL_Event event, JWindow *win);
	
	bool isConsumed();
	void consume();

private:
	bool consumed;
};



#endif


