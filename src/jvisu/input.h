/*
 * Definitions and Declarations of input event objects
 */
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include "sdl.h"
#include "vectormath.h"


class JWindow;
class Layer2D;

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
 * Abstract Button Events
 */

class ButtonEvent : public InputEvent {
public:
	virtual bool isPressed() = 0;
	virtual bool isReleased() = 0;
protected:
	ButtonEvent(SDL_Event event, JWindow *win): InputEvent(event, win) {};
};


/*
 * Keyboard Events
 */

class KeyButtonEvent : public ButtonEvent {
friend class InputEvent;
public:
	const SDL_Keycode key;
	
	virtual std::string getType(){return "KEYBUTTON";};
	
	virtual bool isPressed();
	virtual bool isReleased();
protected:
	KeyButtonEvent(SDL_Event event, JWindow *win);
};


/*
 * Mouse Events
 */

class MouseButtonEvent : public ButtonEvent {
friend class InputEvent;
public:
	const int screenX, screenY;
	
	virtual std::string getType(){return "MOUSEBUTTON";};
	
	Vector2f getViewportCoordinates(){return viewportCoordinates;};
	Vector2f getWorldCoordinates(const Layer2D *layer);
	
	virtual bool isPressed();
	virtual bool isReleased();
	
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


