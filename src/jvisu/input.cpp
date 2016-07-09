#include "sdl.h"
#include "input.h"
#include "window.h"
#include "vectormath.h"
#include "viewport.h"


/*
 * Source for InputEvent
 */
InputEvent *InputEvent::createInputEvent(SDL_Event event, JWindow *win){
	/**
	 * Factory method to produce an event of the appropriate sub-type
	 */
	if(event.type == SDL_QUIT) return new QuitEvent(event, win);
	if(event.type == SDL_MOUSEMOTION){
		return new MouseMotionEvent(event, win);
	}
	if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
		return new MouseButtonEvent(event, win);
	}
	if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
		return new KeyButtonEvent(event, win);
	}
	
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
 * Source for KeyButtonEvent
 */

KeyButtonEvent::KeyButtonEvent(SDL_Event event, JWindow*win):
	ButtonEvent(event, win),
	key(event.key.keysym.sym)
{}


bool KeyButtonEvent::isPressed(){
	return sdlEvent.key.state == SDL_PRESSED;
}

bool KeyButtonEvent::isReleased(){
	return sdlEvent.key.state == SDL_RELEASED;
}


/*
 * Source for MouseButtonEvent
 */

MouseButtonEvent::MouseButtonEvent(SDL_Event event, JWindow *win):
	ButtonEvent(event, win),
	screenX(event.button.x),
	screenY(event.button.y)
{
	// Compute viewport coordinates
	float vx, vy;
	window->screenToViewport(screenX, screenY, vx, vy);
	viewportCoordinates = Vector2f(vx, vy);
}


Vector2f MouseButtonEvent::getWorldCoordinates(const Layer2D *layer){
	Vector2f viewportCoordinates = getViewportCoordinates();
	return layer->viewport.viewportToWorld(viewportCoordinates);
}


bool MouseButtonEvent::isPressed(){
	return sdlEvent.button.state == SDL_PRESSED;
}

bool MouseButtonEvent::isReleased(){
	return sdlEvent.button.state == SDL_RELEASED;
}

bool MouseButtonEvent::isLeftButton(){
	return sdlEvent.button.button == SDL_BUTTON_LEFT;
}

bool MouseButtonEvent::isMiddleButton(){
	return sdlEvent.button.button == SDL_BUTTON_MIDDLE;
}

bool MouseButtonEvent::isRightButton(){
	return sdlEvent.button.button == SDL_BUTTON_RIGHT;
}


/*
 * Source for MouseMotionEvent
 */

MouseMotionEvent::MouseMotionEvent(SDL_Event event, JWindow *win):
	InputEvent(event, win),
	screenX(event.motion.x),
	screenY(event.motion.y),
	deltaX(event.motion.xrel),
	deltaY(event.motion.yrel)
{
	// Compute viewport coordinates
	float vx, vy;
	window->screenToViewport(screenX, screenY, vx, vy);
	viewportCoordinates = Vector2f(vx, vy);
}


Vector2f MouseMotionEvent::getWorldCoordinates(const Layer2D *layer){
	Vector2f viewportCoordinates = getViewportCoordinates();
	return layer->viewport.viewportToWorld(viewportCoordinates);
}


bool MouseMotionEvent::leftButtonPressed(){
	return (sdlEvent.motion.state & SDL_BUTTON_LMASK) != 0;
}

bool MouseMotionEvent::rightButtonPressed(){
	return (sdlEvent.motion.state & SDL_BUTTON_RMASK) != 0;
}

bool MouseMotionEvent::middleButtonPressed(){
	return (sdlEvent.motion.state & SDL_BUTTON_MMASK) != 0;
}


/*
 * Source for QuitEvent
 */
QuitEvent::QuitEvent(SDL_Event event, JWindow *win): InputEvent(event, win) {}

