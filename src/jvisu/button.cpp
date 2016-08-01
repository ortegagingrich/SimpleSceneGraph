/**
 * Notes About Buttons:
	1) Every Layer has an InputManager Object (Maybe the Callback Manager?)
	2) This Object is passed down the scene graph tree through processEvent()
	3) Buttons are just a special type of scene graph component
	4) When buttons process an appropriate input event, they mark it as consumed
	   and give it to the InputManager for delayed priority/processing
	5) When a button is given a consumed event, rather than ignoring, it checks
	   if it is already triggered by another button.  If so, it adds itself to
	   the priority queue.  If it is just consumed with no queue, nothing is done.
	6) Button processing in the priority queue is done at the layer level.  In all
	   lower layers, the event is just passed through as consumed, so there is no
	   danger of accidental activation of another button.
	7) Maybe the queue is not really necessary; if we know that only one button
	   can be activated by an event, we just need to keep track of the one with
	   the highest z-value so far.
	8) On processEvent, no distinguishing between which callback function should
	   be called is done.  It is only determined whether the mouse button event
	   happens inside of the button's rectangle.
	9) When the processing is done at the end of the layer, the chosen button has
	   its precallback() method called.  Based on the event type, etc.
 */

#include <list>

#include "input.h"
#include "window.h"
#include "layer.h"
#include "viewport.h"
#include "scene_graph.h"
#include "button.h"
#include "button_manager.h"
#include "vectormath.h"
#include "geometry.h"


/*
 * ComponentButtonSimple2D
 */

void ComponentButtonSimple2D::update(Layer2D *layer, float tpf){
	ComponentButton2D::update(layer, tpf);
	
	NodeInput2D::update(layer, tpf);
}


void ComponentButtonSimple2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport
){
	// Renderables for self
	ComponentSpriteSimple2D::collectRenderables(render_list, viewport);
	
	/*
	 * Collect Renderables for Children
	 */
	collectChildRenderables(render_list, viewport);
}


void ComponentButtonSimple2D::processEvent(InputEvent *event, Layer2D *layer, float tpf){
	// Process the event for the button
	ComponentButton2D::processEvent(event, layer, tpf);
	
	// Send the event to children for processing
	NodeInput2D::processEvent(event, layer, tpf);
}



bool ComponentButtonSimple2D::isInside(float x, float y, Viewport2D &viewport){
	// We will need absolute positions, so re-calculate them for this component
	computeAbsolutePosition(parent);
	
	Vector2f eventCoordinates, centerpos;
	if(fixedSize){
		eventCoordinates.set(x, y);
		centerpos = viewport.worldToViewport(positionAbsolute);
	}else{
		viewport.viewportToWorld(x, y, eventCoordinates.x, eventCoordinates.y);
		centerpos = positionAbsolute;
	}
	
	eventCoordinates -= centerpos;
	eventCoordinates.rotate(-rotationAbsolute);
	eventCoordinates.add(-centerOffset.x, centerOffset.y);
	
	Rect2f buttonRect(0, width, -height, 0);
	return calculate_intersection(buttonRect, eventCoordinates);
}



/*
 * ComponentButton2D
 */

void ComponentButton2D::update(Layer2D *layer, float tpf){
	
	JWindow *window = layer->getWindow();
	
	/*
	 * Click Processing
	 */
	
	// Get Cursor coordinates
	int mx = window->getMouseX();
	int my = window->getMouseY();
	Vector2f mouseViewport;
	window->screenToViewport(mx, my, mouseViewport.x, mouseViewport.y);
	
	// If the cursor is not on the button, cancel pending clicks
	if(!isInside(mouseViewport, layer->viewport)){
		pendingLeftClick = false;
		pendingRightClick = false;
		pendingMiddleClick = false;
	}
	
	/*
	 * If the pending click buttons are no longer being pressed, the release
	 * happened outside of the button, so there is no longer a pending click on
	 * this button.
	 */
	if(!window->isLeftMouseButtonPressed()) pendingLeftClick = false;
	if(!window->isRightMouseButtonPressed()) pendingRightClick = false;
	if(!window->isMiddleMouseButtonPressed()) pendingMiddleClick = false;
}


void ComponentButton2D::processEvent(InputEvent *e, Layer2D *layer, float tpf){
	if(e == NULL) return;
	if(e->getType() == "MOUSEBUTTON"){
		MouseButtonEvent *event = (MouseButtonEvent*) e;
	
		if(isInside(event->getViewportCoordinates(), layer->viewport)){
			layer->buttonManager.considerButton(this, zLevel, event);
		}
	}else if(e->getType() == "MOUSEMOTION"){
		MouseMotionEvent *event = (MouseMotionEvent*) e;
		
		if(isInside(event->getViewportCoordinates(), layer->viewport)){
			layer->buttonManager.considerButton(this, zLevel, event);
		}else if(mouseAlreadyOver){
			onEndMouseOver(event, tpf);
			mouseAlreadyOver = false;
		}
	}
	
}



void ComponentButton2D::precallback(InputEvent *e, float tpf){
	if(e == NULL) return;
	
	// If it is a new mouseover event
	if(e->getType() == "MOUSEMOTION" && !mouseAlreadyOver){
		onStartMouseOver((MouseMotionEvent*) e, tpf);
		mouseAlreadyOver = true;
	}
	
	
	// It must be a MouseButtonEvent, so branch appropriately
	if(e->getType() != "MOUSEBUTTON") return;
	MouseButtonEvent *event = (MouseButtonEvent*) e;
	
	if(event->isPressed()){
		if(event->isLeftButton()){
			onLeftPress(event, tpf);
			pendingLeftClick = true;
		}else if(event->isRightButton()){
			onRightPress(event, tpf);
			pendingRightClick = true;
		}else if(event->isMiddleButton()){
			onMiddlePress(event, tpf);
			pendingMiddleClick = true;
		}
	}
	
	if(event->isReleased()){
		if(event->isLeftButton()){
			onLeftRelease(event, tpf);
			if(pendingLeftClick) onLeftClick(event, tpf);
		}else if(event->isRightButton()){
			onRightRelease(event, tpf);
			if(pendingRightClick) onRightClick(event, tpf);
		}else if(event->isMiddleButton()){
			onMiddleRelease(event, tpf);
			if(pendingMiddleClick) onMiddleClick(event, tpf);
		}
	}
	
	
	/*
	 * TODO: Split based on event properties, etc. and call the appropriate
	 * callback function.
	 */
}




/*
 * ButtonManager
 */

ButtonManager::ButtonManager():
	topButton(NULL)
{}


void ButtonManager::considerButton(
	ComponentButton2D *button,
	float priority,
	InputEvent *event
){
	/**
	 * Checks to see if the provided button has higher priority than the stored
	 * button.  If so, it replaces the stored button.
	 */
	
	if(topButton == NULL){
		if(event->isConsumed()) return;
	}else if(priority <= topPriority){
		return;
	}
	topButton = button;
	topPriority = priority;
	event->consume();
}


void ButtonManager::processEvent(InputEvent *event, float tpf){
	/**
	 * Initiates the pre-callback
	 */
	if(topButton != NULL){
		topButton->precallback(event, tpf);
	}
	
	topButton = NULL;
}


