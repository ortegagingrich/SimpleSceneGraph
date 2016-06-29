#include "sdl.h"
#include "input.h"
#include "window.h"


/*
 * Source for InputEvent
 */
InputEvent *InputEvent::createInputEvent(SDL_Event event, JWindow *win){
	// Temporary: just make the generic type
	return new InputEvent(event, win);
}

InputEvent::InputEvent(SDL_Event event, JWindow *win):
	window(win),
	sdlEvent(event),
	consumed(false)
{}


bool InputEvent::isConsumed(){return consumed;}
void InputEvent::consume(){consumed = true;}


