#include "sdl.h"
#include "input.h"
#include "window.h"


/*
 * Source for InputEvent
 */
InputEvent *InputEvent::createInputEvent(SDL_Event event, JWindow *win){
	/**
	 * Factory method to produce an event of the appropriate sub-type
	 */
	if(event.type == SDL_QUIT) return new QuitEvent(event, win);
	
	// If all else fails, just make the generic type
	return new InputEvent(event, win);
}

InputEvent::InputEvent(SDL_Event event, JWindow *win):
	window(win),
	sdlEvent(event),
	consumed(false)
{}


bool InputEvent::isConsumed(){return consumed;}
void InputEvent::consume(){consumed = true;}


/*
 * Source for QuitEvent
 */
QuitEvent::QuitEvent(SDL_Event event, JWindow *win): InputEvent(event, win) {}

