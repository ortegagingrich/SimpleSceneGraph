#ifndef BUTTON_H
#define BUTTON_H

#include <list>

#include "callback.h"
#include "scene_graph.h"
#include "button_manager.h"
#include "vectormath.h"


class Layer2D;
class Viewport2D;
class Texture;

class ComponentButtonSimple2D;



class SHARED_EXPORT ComponentButton2D:
	virtual public Component2D,
	virtual protected JEventCallback
{
friend class ButtonManager;
friend class ComponentButtonSimple2D;
friend class ComponentDraggable2D;
public:
	
	ComponentButton2D();
	
	// Necessary to keep track of past input events for click, double click, etc.
	virtual void update(Layer2D *layer, float tpf);
	
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
	
	virtual void callback(InputEvent *event, float tpf){};
	
	virtual bool isPressed() const;
	
	// Abstract Callback Methods
	// Note: More than one of these might be called if they overlap
	
	// Only called upon first Press
	virtual void onLeftPress(MouseButtonEvent *event, float tpf){};
	virtual void onRightPress(MouseButtonEvent *event, float tpf){};
	virtual void onMiddlePress(MouseButtonEvent *event, float tpf){};
	
	// Called on Release
	virtual void onLeftRelease(MouseButtonEvent *event, float tpf){};
	virtual void onRightRelease(MouseButtonEvent *event, float tpf){};
	virtual void onMiddleRelease(MouseButtonEvent *event, float tpf){};
	
	// Press and Release happened on the same button
	virtual void onLeftClick(MouseButtonEvent *event, float tpf){};
	virtual void onRightClick(MouseButtonEvent *event, float tpf){};
	virtual void onMiddleClick(MouseButtonEvent *event, float tpf){};
	
	// Double-Click; also generages Single Click
	virtual void onLeftDoubleClick(MouseButtonEvent *event, float tpf){};
	virtual void onRightDoubleClick(MouseButtonEvent *event, float tpf){};
	virtual void onMiddleDoubleClick(MouseButtonEvent *event, float tpf){};
	
	// Mouseover; Only activated the first frame the mouse is over the button
	virtual void onStartMouseOver(MouseMotionEvent *event, float tpf){};
	virtual void onEndMouseOver(MouseMotionEvent *event, float tpf){};
	
protected:
	virtual bool isInside(float x, float y, Viewport2D &viewport) = 0;
	virtual bool isInside(Vector2f vc, Viewport2D &viewport){
		return isInside(vc.x, vc.y, viewport);
	};
	
	
	virtual void precallback(InputEvent *event, float tpf);
	
	// Internal Pre-callback functions.
	virtual void preLeftPress(MouseButtonEvent *event, float tpf);
	virtual void preRightPress(MouseButtonEvent *event, float tpf);
	virtual void preMiddlePress(MouseButtonEvent *event, float tpf);
	
	virtual void preLeftRelease(MouseButtonEvent *event, float tpf);
	virtual void preRightRelease(MouseButtonEvent *event, float tpf);
	virtual void preMiddleRelease(MouseButtonEvent *event, float tpf);
	
	virtual void preLeftClick(MouseButtonEvent *event, float tpf);
	virtual void preRightClick(MouseButtonEvent *event, float tpf);
	virtual void preMiddleClick(MouseButtonEvent *event, float tpf);
	
	virtual void preLeftDoubleClick(MouseButtonEvent *event, float tpf);
	virtual void preRightDoubleClick(MouseButtonEvent *event, float tpf);
	virtual void preMiddleDoubleClick(MouseButtonEvent *event, float tpf);
	
	virtual void preStartMouseOver(MouseMotionEvent *event, float tpf);
	virtual void preEndMouseOver(MouseMotionEvent *event, float tpf);
	
	
private:
	bool pendingLeftClick, pendingRightClick, pendingMiddleClick;
	bool mouseAlreadyOver;
};






class SHARED_EXPORT ComponentButtonSimple2D:
	virtual public ComponentButton2D,
	virtual public ComponentSpriteSimple2D,
	virtual public NodeInput2D
{
public:
	
	ComponentButtonSimple2D(JWindow *win);
	~ComponentButtonSimple2D();
	
	Texture *getOverlayTexture() const;
	Texture *getPressedTexture() const;
	void setOverlayTexture(Texture *tex);
	void setPressedTexture(Texture *tex);
	
	virtual void removeTexture(Texture *tex);
	
	
	// Text Overlay Methods
	void setText(std::string text);
	void clearText();
	void setFont(std::string font);
	void setTextColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	
	
	
	virtual void update(Layer2D *layer, float tpf);
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);

protected:
	virtual bool isInside(float x, float y, Viewport2D &viewport);

private:
	Texture *overlayTexture, *pressedTexture;
	ComponentSpriteText2D *textOverlay;
};




class SHARED_EXPORT ComponentDraggable2D:
	public ComponentButtonSimple2D
{
public:
	
	ComponentDraggable2D(JWindow *win);
	
	virtual void update(Layer2D *layer, float tpf);
	
	virtual void onDragStart(float tpf){};
	virtual void onDragEnd(float tpf){};
	
protected:
	virtual void preLeftPress(MouseButtonEvent *event, float tpf);
	virtual void preDragStart(float tpf);
	virtual void preDragEnd(float tpf);
	
private:
	Vector2f anchor;
};



#endif
