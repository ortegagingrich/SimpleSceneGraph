/*
 * Definitions and Declarations of input event objects
 */
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include "sdl.h"
#include "vectormath.h"


class JWindow;
class Viewport2D;

/*
 * Base Class
 */

class InputEvent {
public:
	// Factory Method
	static InputEvent *createInputEvent(SDL_Event event, JWindow *win);
	
	
	virtual ~InputEvent(){};
	
	const JWindow *window;
	const SDL_Event sdlEvent;
	
	virtual std::string getType(){return "NONE";};
	
	bool isConsumed();
	void consume();

protected:
	InputEvent(SDL_Event event, JWindow *win);
private:
	bool consumed;
};



/*
 * Mouse Events
 */

class MouseButtonEvent : public InputEvent {
friend class InputEvent;
public:
	const int screenX, screenY;
	
	virtual std::string getType(){return "MOUSEBUTTON";};
	
	Vector2f getViewportCoordinates(){return viewportCoordinates;};
	Vector2f getWorldCoordinates(const Viewport2D *viewport);
	
	bool isPressed();
	bool isReleased();
	
	bool isLeftButton();
	bool isRightButton();
	bool isMiddleButton();
protected:
	MouseButtonEvent(SDL_Event event, JWindow *win);
	
	Vector2f viewportCoordinates;
};




/*
 * Quit Event
 */

class QuitEvent : public InputEvent {
friend class InputEvent;
public:
	virtual std::string getType(){return "QUIT";};
protected:
	QuitEvent(SDL_Event event, JWindow *win);
};




#endif


