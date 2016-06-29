/*
 * Definitions and Declarations of input event objects
 */
#ifndef INPUT_H
#define INPUT_H

#include "sdl.h"

class JWindow;

/*
 * Base Class
 */

class InputEvent {
public:
	// Factory Method
	static InputEvent *createInputEvent(SDL_Event event, JWindow *win);
	
	const JWindow *window;
	const SDL_Event sdlEvent;
	
	bool isConsumed();
	void consume();

protected:
	InputEvent(SDL_Event event, JWindow *win);
private:
	bool consumed;
};

/*
 * Quit Event
 */

class QuitEvent : public InputEvent {
friend class InputEvent;
protected:
	QuitEvent(SDL_Event event, JWindow *win);
};





#endif


