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
		
		virtual void callback(MouseButtonEvent *event, float tpf){
			if(event->isLeftButton()) activated = true;
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event, float tpf){
			FAIL();
		};
	};
	
	class CallbackRightMouse : public MouseButtonCallback {
	public:
		bool activated;
		CallbackRightMouse(JWindow *win): MouseButtonCallback(win), activated(false){};
		
		void reset(){activated = false;};
		
		virtual void callback(MouseButtonEvent *event, float tpf){
			if(event->isRightButton()) activated = true;
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event, float tpf){
			FAIL();
		};
	};
	
	class CallbackKey : public KeyButtonCallback {
	public:
		bool activated;
		CallbackKey(JWindow *win): KeyButtonCallback(win), activated(false){};
		
		void reset(){activated = false;};
		
		virtual void callback(KeyButtonEvent *event, float tpf){
			activated = true;
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event, float tpf){
			FAIL();
		};
	};
	
	/*
	 * Make Window
	 */
	JWindow *window = new JWindow(100, 100, false);
	
	CallbackLeftMouse *clm = new CallbackLeftMouse(window);
	CallbackRightMouse *crm = new CallbackRightMouse(window);
	CallbackKey *ck = new CallbackKey(window);
	
	SDL_Event sdlEvent;
	
	
	// Case 1 : Quit Event (should trigger none)
	sdlEvent.type = SDL_QUIT;
	
	window->processEvent(sdlEvent, 1.0f);
	
	EXPECT_EQ(clm->activated, false);
	EXPECT_EQ(crm->activated, false);
	EXPECT_EQ(ck->activated, false);
	
	clm->reset();
	crm->reset();
	ck->reset();
	window->activate();
	
	
	// Case 2 : Key Event (should trigger only ck)
	sdlEvent.type = SDL_KEYDOWN;
	
	window->processEvent(sdlEvent, 0.0f);
	
	EXPECT_EQ(clm->activated, false);
	EXPECT_EQ(crm->activated, false);
	EXPECT_EQ(ck->activated, true);
	
	clm->reset();
	crm->reset();
	ck->reset();
	
	
	// Case 3 : Left Mouse Event (should trigger only clm)
	sdlEvent.type = SDL_MOUSEBUTTONUP;
	sdlEvent.button.button = SDL_BUTTON_LEFT;
	
	window->processEvent(sdlEvent, 0.0f);
	
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



TEST(Input, EventPriority){
	/**
	 * This test is to ensure that callbacks are given their expected priorities
	 * when handling events which match multiple ones.
	 * 
	 * Priorities are designed as follows:
	 * 1) If two callbacks are in different layers, the one in the higher layer
	 *    gets priority.
	 * 2) Within a layer, callbacks in the scene graph receive priority over
	 *    those attached to the layer itself.
	 * 3) Callbacks attached to the window receive lowest priority.
	 * 4) Callbacks both in the same scene graph, both attached directly to the
	 *    same layer or both attached to the same window have the have the
	 *    same priority.  (TODO: explicit priorities?)
	 * 5) A callback with higher priority will always be called before a 
	 *    callback with lower priority.
	 * 6) The order in which callbacks of the same priority are called should
	 *    be considered non-deterministic.
	 */
	
	/*
	 * Callback definition
	 */
	
	class PriorityCallback : public KeyButtonCallback {
	public:
		bool activated;
		
		PriorityCallback(JWindow *win): KeyButtonCallback(win), activated(false){};
		PriorityCallback(Layer *l): KeyButtonCallback(l), activated(false){};
		PriorityCallback(ComponentInput2D *c): KeyButtonCallback(c), activated(false){};
		
		void reset(){activated = false;};
		
		virtual void callback(KeyButtonEvent *event, float tpf){
			activated = true;
			event->consume();
		};
		
		// If this method is called for any reason, automatic failure
		virtual void callback(InputEvent *event, float tpf){
			FAIL();
		};
	};
	
	/*
	 * Setup
	 */
	
	JWindow *window = new JWindow(100, 100, false);
	
	Layer2D *layer1, *layer2;
	NodeInput2D *node;
	
	layer1 = new Layer2D("Layer 1");
	layer2 = new Layer2D("Layer 2");
	node = new NodeInput2D();
	
	layer2->rootNode->attachChild(node);
	window->addLayerBottom(layer2);
	window->addLayerTop(layer1);
	
	SDL_Event sdlEvent;
	sdlEvent.type = SDL_KEYDOWN;
	
	
	/*
	 * Add callbacks one at a time, from expected lowest priority to highest.
	 * Each time, process the SDL event to make sure that only the callback with
	 * the highest priority is activated.
	 */
	
	// Trial 1
	
	PriorityCallback *c1 = new PriorityCallback(window);
	window->processEvent(sdlEvent, 0.0f);
	
	EXPECT_EQ(c1->activated, true);
	
	c1->reset();
	
	// Trial 2
	
	PriorityCallback *c2 = new PriorityCallback(layer2);
	window->processEvent(sdlEvent, 0.0f);
	
	EXPECT_EQ(c1->activated, false);
	EXPECT_EQ(c2->activated, true);
	
	c1->reset();
	c2->reset();
	
	// Trial 3
	
	PriorityCallback *c3 = new PriorityCallback(node);
	window->processEvent(sdlEvent, 0.0f);
	
	EXPECT_EQ(c1->activated, false);
	EXPECT_EQ(c2->activated, false);
	EXPECT_EQ(c3->activated, true);
	
	c1->reset();
	c2->reset();
	c3->reset();
	
	// Trial 4
	
	PriorityCallback *c4 = new PriorityCallback(layer1);
	window->processEvent(sdlEvent, 0.0f);
	
	EXPECT_EQ(c1->activated, false);
	EXPECT_EQ(c2->activated, false);
	EXPECT_EQ(c3->activated, false);
	EXPECT_EQ(c4->activated, true);
	
	c1->reset();
	c2->reset();
	c3->reset();
	c4->reset();
	
	
	/*
	 * Cleanup
	 */
	delete window;
}






































