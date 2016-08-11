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

#include <cstdio>
#include <list>

#include "input.h"
#include "window.h"
#include "layer.h"
#include "viewport.h"
#include "scene_graph.h"
#include "text.h"
#include "button.h"
#include "button_manager.h"
#include "renderable.h"
#include "vectormath.h"
#include "geometry.h"


/*
 * ComponentButtonSimple2D
 */

ComponentButtonSimple2D::ComponentButtonSimple2D(JWindow *win):
	overlayTexture(NULL),
	pressedTexture(NULL)
{
	textOverlay = new ComponentSpriteText2D(win);
	textOverlay->parent = this;
}

ComponentButtonSimple2D::~ComponentButtonSimple2D(){
	if(overlayTexture != NULL) overlayTexture->removeOwner(this);
	if(pressedTexture != NULL) pressedTexture->removeOwner(this);
	
	if(textOverlay != NULL) delete textOverlay;
}


Texture *ComponentButtonSimple2D::getOverlayTexture() const {
	return overlayTexture;
}

Texture *ComponentButtonSimple2D::getPressedTexture() const {
	return pressedTexture;
}

void ComponentButtonSimple2D::setOverlayTexture(Texture *tex){
	if(overlayTexture != NULL) overlayTexture->removeOwner(this);
	if(tex != NULL) tex->addOwner(this);
	overlayTexture = tex;
}

void ComponentButtonSimple2D::setPressedTexture(Texture *tex){
	if(pressedTexture != NULL) pressedTexture->removeOwner(this);
	if(tex != NULL) tex->addOwner(this);
	pressedTexture = tex;
}


void ComponentButtonSimple2D::removeTexture(Texture *tex){
	// Super method to check the base texture
	ComponentSpriteSimple2D::removeTexture(tex);
	
	//Check other textures
	if(overlayTexture != NULL) overlayTexture = NULL;
	if(pressedTexture != NULL) pressedTexture = NULL;
}


// Text Methods
void ComponentButtonSimple2D::setText(std::string text){
	textOverlay->text = text;
}

void ComponentButtonSimple2D::clearText(){
	textOverlay->text = "";
}

void ComponentButtonSimple2D::setFont(std::string font){
	textOverlay->fontPath = font;
}

void ComponentButtonSimple2D::setFontSize(int size){
	textOverlay->fontSize = size;
}

void ComponentButtonSimple2D::setTextColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	textOverlay->colorRed = r;
	textOverlay->colorGreen = g;
	textOverlay->colorBlue = b;
	textOverlay->colorAlpha = a;
}




void ComponentButtonSimple2D::update(Layer2D *layer, float tpf){
	ComponentButton2D::update(layer, tpf);
	
	if(textOverlay != NULL) textOverlay->update(layer, tpf);
	
	NodeInput2D::update(layer, tpf);
}


void ComponentButtonSimple2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport
){
	// Renderables for self
	// Base Texture
	if(isHidden()) return;
	
	
	ComponentSpriteSimple2D::collectRenderables(render_list, viewport);
	
	
	// Other Textures
	RenderableSprite *overlaySprite, *pressedSprite;
	
	if(overlayTexture != NULL && mouseAlreadyOver){
		overlaySprite = makeRenderableFromTexture(overlayTexture, viewport);
		
		if(overlaySprite != NULL){
			overlaySprite->zMod = 1.0f;
			render_list.push_back(overlaySprite);
		}
	}
	
	if(pressedTexture != NULL && pendingLeftClick){
		pressedSprite = makeRenderableFromTexture(pressedTexture, viewport);
		
		if(pressedSprite != NULL){
			pressedSprite->zMod = 2.0f;
			render_list.push_back(pressedSprite);
		}
	}
	
	// Overlay Text
	textOverlay->centerOffset = centerOffset;
	textOverlay->collectRenderables(render_list, viewport, 3.0f);
	
	
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



bool ComponentButtonSimple2D::isInside(float x, float y, Layer2D *layer){
	/**
	 * Check whether or not the provided coordinates (which use viewport coordinates)
	 * are inside of the sprite's rectangle
	 */
	if(layer == NULL) return false;
	
	
	// We will need absolute positions, so re-calculate them for this component
	computeAbsolutePosition(parent);
	
	
	// No need to do anything if the rectangle is collapsed
	if(width * height * scaleAbsolute.x * scaleAbsolute.y == 0) return false;
	
	
	/*
	 * Cases we must be able to deal with:
	 * 1) Both width and height are negative (image)
	 * 2) Only one of width and height is negative:
	 *    a) fixedSize
	 *    b) !fixedSize
	 * 3) Both width and height are positive:
	 *    a) fixedSize
	 *    b) !fixedSize
	 */
	
	// Width and Height which reflect the actual width and height of the sprite
	Texture *texture = getTexture();
	bool fixedPixel = false;
	float w = width, h = -height;
	if(width < 0){
		if(texture == NULL) return false;
		if(height < 0){
			w = texture->width;
			h = -texture->height;
			fixedPixel = true;
		}else{
			w = h * texture->getAspectRatio();
		}
	}else if(height < 0){
		if(texture == NULL) return false;
		h = -w / texture->getAspectRatio();
	}
	
	
	Vector2f eventCoordinates, centerpos;
	if(fixedSize){
		eventCoordinates.set(x, y);
		centerpos = layer->viewport.worldToViewport(positionAbsolute);
	}else{
		layer->viewport.viewportToWorld(x, y, eventCoordinates.x, eventCoordinates.y);
		centerpos = positionAbsolute;
	}
	
	
	eventCoordinates -= centerpos;
	eventCoordinates.rotate(-rotationAbsolute);
	eventCoordinates.add(-centerOffset.x, centerOffset.y);
	
	if(fixedPixel){
		if(layer->getWindow() == NULL) return false;
		float pixelFactor = layer->getWindow()->getScreenHeight() / 2;
		
		eventCoordinates.scale(pixelFactor);
	}
	
	Rect2f buttonRect(0, w * scaleAbsolute.x, h * scaleAbsolute.y, 0);
	return calculate_intersection(buttonRect, eventCoordinates);
}



/*
 * ComponentButton2D
 */

ComponentButton2D::ComponentButton2D():
	pendingLeftClick(false),
	pendingRightClick(false),
	pendingMiddleClick(false),
	mouseAlreadyOver(false)
{}


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
	if(!isInside(mouseViewport, layer)){
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
	
		if(isInside(event->getViewportCoordinates(), layer)){
			layer->buttonManager.considerButton(this, zLevel, event);
		}
	}else if(e->getType() == "MOUSEMOTION"){
		MouseMotionEvent *event = (MouseMotionEvent*) e;
		
		if(isInside(event->getViewportCoordinates(), layer)){
			layer->buttonManager.considerButton(this, zLevel, event);
		}else if(mouseAlreadyOver){
			preEndMouseOver(event, tpf);
			mouseAlreadyOver = false;
		}
	}
	
}


bool ComponentButton2D::isPressed() const {
	return pendingLeftClick;
}


void ComponentButton2D::precallback(InputEvent *e, float tpf){
	if(e == NULL) return;
	
	// If it is a new mouseover event
	if(e->getType() == "MOUSEMOTION" && !mouseAlreadyOver){
		preStartMouseOver((MouseMotionEvent*) e, tpf);
		mouseAlreadyOver = true;
	}
	
	
	// It must be a MouseButtonEvent, so branch appropriately
	if(e->getType() != "MOUSEBUTTON") return;
	MouseButtonEvent *event = (MouseButtonEvent*) e;
	
	if(event->isPressed()){
		if(event->isLeftButton()){
			preLeftPress(event, tpf);
			pendingLeftClick = true;
		}else if(event->isRightButton()){
			preRightPress(event, tpf);
			pendingRightClick = true;
		}else if(event->isMiddleButton()){
			preMiddlePress(event, tpf);
			pendingMiddleClick = true;
		}
	}
	
	if(event->isReleased()){
		if(event->isLeftButton()){
			preLeftRelease(event, tpf);
			if(pendingLeftClick) onLeftClick(event, tpf);
		}else if(event->isRightButton()){
			preRightRelease(event, tpf);
			if(pendingRightClick) onRightClick(event, tpf);
		}else if(event->isMiddleButton()){
			preMiddleRelease(event, tpf);
			if(pendingMiddleClick) onMiddleClick(event, tpf);
		}
	}
	
	
	/*
	 * TODO: Split based on event properties, etc. and call the appropriate
	 * callback function.
	 */
}


/*
 * Precallback Hooks
 */
void ComponentButton2D::preLeftPress(MouseButtonEvent *event, float tpf){
	onLeftPress(event, tpf);
}

void ComponentButton2D::preRightPress(MouseButtonEvent *event, float tpf){
	onRightPress(event, tpf);
}

void ComponentButton2D::preMiddlePress(MouseButtonEvent *event, float tpf){
	onMiddlePress(event, tpf);
}

void ComponentButton2D::preLeftRelease(MouseButtonEvent *event, float tpf){
	onLeftRelease(event, tpf);
}

void ComponentButton2D::preRightRelease(MouseButtonEvent *event, float tpf){
	onRightRelease(event, tpf);
}

void ComponentButton2D::preMiddleRelease(MouseButtonEvent *event, float tpf){
	onMiddleRelease(event, tpf);
}

void ComponentButton2D::preLeftClick(MouseButtonEvent *event, float tpf){
	onLeftClick(event, tpf);
}

void ComponentButton2D::preRightClick(MouseButtonEvent *event, float tpf){
	onRightClick(event, tpf);
}

void ComponentButton2D::preMiddleClick(MouseButtonEvent *event, float tpf){
	onMiddleClick(event, tpf);
}

void ComponentButton2D::preLeftDoubleClick(MouseButtonEvent *event, float tpf){
	onLeftDoubleClick(event, tpf);
}

void ComponentButton2D::preRightDoubleClick(MouseButtonEvent *event, float tpf){
	onRightDoubleClick(event, tpf);
}

void ComponentButton2D::preMiddleDoubleClick(MouseButtonEvent *event, float tpf){
	onMiddleDoubleClick(event, tpf);
}

void ComponentButton2D::preStartMouseOver(MouseMotionEvent *event, float tpf){
	onStartMouseOver(event, tpf);
}

void ComponentButton2D::preEndMouseOver(MouseMotionEvent *event, float tpf){
	onEndMouseOver(event, tpf);
}



/*
 * ComponentDraggable2D
 */

ComponentDraggable2D::ComponentDraggable2D(JWindow *win):
	ComponentButtonSimple2D(win),
	anchor(0.0f, 0.0f)
{}


void ComponentDraggable2D::update(Layer2D *layer, float tpf){
	// If there is a pending left click, a drag must be in progress
	if(pendingLeftClick){
		// Get the current world coordinates of the mouse cursor
		JWindow *window = layer->getWindow();
		int sx = window->getMouseX();
		int sy = window->getMouseY();
		Vector2f vc, wc;
		window->screenToViewport(sx, sy, vc.x, vc.y);
		wc = layer->viewport.viewportToWorld(vc);
		
		Vector2f rel;
		if(inheritPosition && parent != NULL){
			rel = parent->computeRelativePosition(wc);
		}else{
			rel = wc;
		}
		
		position = rel + anchor;
	}
	
	ComponentButtonSimple2D::update(layer, tpf);
}


void ComponentDraggable2D::preLeftPress(MouseButtonEvent *event, float tpf){
	// Choose a new anchor point
	Layer2D *layer = getLayer();
	if(layer == NULL) return;
	Vector2f wc, rel;
	wc = event->getWorldCoordinates(layer);
	if(inheritPosition && parent != NULL){
		rel = parent->computeRelativePosition(wc);
	}else{
		rel = wc;
	}
	anchor = position - rel;
	
	
	ComponentButton2D::preLeftPress(event, tpf);
	preDragStart(tpf);
}

void ComponentDraggable2D::preDragStart(float tpf){
	onDragStart(tpf);
}

void ComponentDraggable2D::preDragEnd(float tpf){
	onDragEnd(tpf);
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


