#include "sdl.h"
#include "input.h"


/*
 * Source for InputEvent
 */
InputEvent *InputEvent::createInputEvent(SDL_Event event){
	// Temporary: just make the generic type
	return new InputEvent(event);
}

 InputEvent::InputEvent(SDL_Event event): sdlEvent(event), consumed(false) {}

bool InputEvent::isConsumed(){return consumed;}
void InputEvent::consume(){consumed = true;}


