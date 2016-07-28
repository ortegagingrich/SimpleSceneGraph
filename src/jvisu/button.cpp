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
#include "scene_graph.h"
#include "button.h"


/*
 * ComponentButtonSimple2D
 */

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


void ComponentButtonSimple2D::processEvent(InputEvent *event, float tpf){
	
	//TODO: Insert button event processing
	
	NodeInput2D::processEvent(event, tpf);
}


void ComponentButtonSimple2D::precallback(InputEvent *event, float tpf){
	/*
	 * TODO: Split based on event properties, etc. and call the appropriate
	 * callback function.
	 */
}
































