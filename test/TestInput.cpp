/*
 * Unit Tests for basic input events.
 * 
 * In general, these tests create SDL_Event struct objects and pass them to a 
 * window with pre-defined callbacks registered to them to make sure that they
 * are called appropriately.
 */
#include <cstdio>
#include <gtest/gtest.h>

#include "../src/jvisu/jvisu.h"
#include "../src/jvisu/window.h"
#include "../src/jvisu/sdl.h"


TEST(Input, MultipleButtonCallbacks){
	/**
	 * This test involves two callbacks defined for mouse button events and one
	 * for key events.  One of the mouse event callbacks activates on left-click 
	 * events, the other on right-click events.  After SDL_Events for clicks and 
	 * button presses are applied, the test verifiesthat the correct callbacks 
	 * were activated.
	 */
	
	/*
	 * Define Callbacks
	 */
	class CallbackLeftMouse : public MouseButtonCallback {
	public:
		bool activated;
		CallbackLeftMouse(JWindow *win): MouseButtonCallback(win), activated(false){};
		
		void reset(){activated = false;};
		
		virtual void callback(MouseButtonEvent *event){
			if(event->isLeftButton()) activated = true;
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event){
			FAIL();
		};
	};
	
	class CallbackRightMouse : public MouseButtonCallback {
	public:
		bool activated;
		CallbackRightMouse(JWindow *win): MouseButtonCallback(win), activated(false){};
		
		void reset(){activated = false;};
		
		virtual void callback(MouseButtonEvent *event){
			if(event->isRightButton()) activated = true;
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event){
			FAIL();
		};
	};
	
	class CallbackKey : public KeyButtonCallback {
	public:
		bool activated;
		CallbackKey(JWindow *win): KeyButtonCallback(win), activated(false){};
		
		void reset(){activated = false;};
		
		virtual void callback(KeyButtonEvent *event){
			activated = true;
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event){
			FAIL();
		};
	};
	
	/*
	 * Make Window
	 */
	JWindow *window = new JWindow(100, 100, "TEST_INPUT");
	
	CallbackLeftMouse *clm = new CallbackLeftMouse(window);
	CallbackRightMouse *crm = new CallbackRightMouse(window);
	CallbackKey *ck = new CallbackKey(window);
	
	SDL_Event sdlEvent;
	
	
	// Case 1 : Quit Event (should trigger none)
	sdlEvent.type = SDL_QUIT;
	
	window->processEvent(sdlEvent);
	
	EXPECT_EQ(clm->activated, false);
	EXPECT_EQ(crm->activated, false);
	EXPECT_EQ(ck->activated, false);
	
	clm->reset();
	crm->reset();
	ck->reset();
	
	
	// Case 2 : Key Event (should trigger only ck)
	sdlEvent.type = SDL_KEYDOWN;
	
	window->processEvent(sdlEvent);
	
	EXPECT_EQ(clm->activated, false);
	EXPECT_EQ(crm->activated, false);
	EXPECT_EQ(ck->activated, true);
	
	clm->reset();
	crm->reset();
	ck->reset();
	
	
	// Case 2 : Left Mouse Event (should trigger only clm)
	sdlEvent.type = SDL_MOUSEBUTTONUP;
	sdlEvent.button.button = SDL_BUTTON_LEFT;
	
	window->processEvent(sdlEvent);
	
	EXPECT_EQ(clm->activated, true);
	EXPECT_EQ(crm->activated, false);
	EXPECT_EQ(ck->activated, false);
	
	clm->reset();
	crm->reset();
	ck->reset();
	
	
	/*
	 * Cleanup
	 */
	delete window; // Automatically takes care of callbacks
	
}




