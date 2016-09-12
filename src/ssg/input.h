/*
 * Definitions and Declarations of input event objects
 */
#ifndef INPUT_H
#define INPUT_H

#include "shared_exports.h"

#include <string>
#include "sdl.h"
#include "vectormath.h"



namespace ssg {
	/*
	 * Event-Based Input
	 */

	class Window;
	class Layer2D;
	class Viewport2D;


	/*
	 * Base Class
	 */

	class SHARED_EXPORT InputEvent {
	public:
		// Factory Method
		static InputEvent *createInputEvent(SDL_Event event, Window *win);
	
	
		virtual ~InputEvent(){};
		
	internal:
		const Window *window;
		const SDL_Event sdlEvent;
		
	public:
		
		virtual std::string getType(){return "NONE";};
	
		virtual bool isConsumed();
		virtual void consume();

	protected:
		InputEvent(SDL_Event event, Window *win);
	private:
		bool consumed;
	};


	class SHARED_EXPORT InputEventPermanent : public InputEvent {
		/**
		 * Input events which cannot be consumed
		 */
	public:
		virtual bool isConsumed();
		virtual void consume();
	protected:
		InputEventPermanent(SDL_Event event, Window *win) : InputEvent(event, win){};
	};


	/*
	 * Abstract Button Events
	 */

	class SHARED_EXPORT ButtonEvent : public InputEvent {
	public:
		virtual bool isPressed() = 0;
		virtual bool isReleased() = 0;
	protected:
		ButtonEvent(SDL_Event event, Window *win): InputEvent(event, win) {};
	};


	/*
	 * Keyboard Events
	 */

	class SHARED_EXPORT KeyButtonEvent : public ButtonEvent {
	friend class InputEvent;
	public:
		const SDL_Keycode key;
	
		virtual std::string getType(){return "KEYBUTTON";};
	
		virtual bool isPressed();
		virtual bool isReleased();
	protected:
		KeyButtonEvent(SDL_Event event, Window *win);
	};


	/*
	 * Mouse Events
	 */

	class SHARED_EXPORT MouseButtonEvent : public ButtonEvent {
	friend class InputEvent;
	public:
		const int screenX, screenY;
	
		virtual std::string getType(){return "MOUSEBUTTON";};
	
		Vector2f getViewportCoordinates(){return viewportCoordinates;};
		Vector2f getWorldCoordinates(const Layer2D *layer);
		Vector2f getWorldCoordinates(const Viewport2D &viewport);
	
		virtual bool isPressed();
		virtual bool isReleased();
	
		bool isLeftButton();
		bool isRightButton();
		bool isMiddleButton();
	protected:
		MouseButtonEvent(SDL_Event event, Window *win);
	
		Vector2f viewportCoordinates;
	};


	class SHARED_EXPORT MouseMotionEvent : public InputEvent {
	friend class InputEvent;
	public:
		const int screenX, screenY;
		const int deltaX, deltaY;
	
		virtual std::string getType(){return "MOUSEMOTION";};
	
		Vector2f getViewportCoordinates(){return viewportCoordinates;};
		Vector2f getWorldCoordinates(const Layer2D *layer);
		Vector2f getWorldCoordinates(const Viewport2D &viewport);
	
		bool leftButtonPressed();
		bool rightButtonPressed();
		bool middleButtonPressed();
	protected:
		MouseMotionEvent(SDL_Event event, Window *win);
	
		Vector2f viewportCoordinates;
	};



	/*
	 * Quit Event
	 */

	class SHARED_EXPORT QuitEvent : public InputEventPermanent {
	friend class InputEvent;
	public:
		virtual std::string getType(){return "QUIT";};
	protected:
		QuitEvent(SDL_Event event, Window *win);
	};


}

#endif


