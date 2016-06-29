/*
 * Definitions and Declarations of input event objects
 */
#ifndef INPUT_H
#define INPUT_H

#include "sdl.h"


class InputEvent {
public:
	// Factory Method
	static InputEvent *createInputEvent(SDL_Event event);
	
	SDL_Event sdlEvent;
	
	InputEvent(SDL_Event event);
	
	bool isConsumed();
	void consume();

private:
	bool consumed;
};



#endif


