#ifndef BUTTON_H
#define BUTTON_H

#include <list>
#include "callback.h"
#include "scene_graph.h"



class SHARED_EXPORT ComponentButtonSimple2D:
	virtual public ComponentSpriteSimple2D,
	virtual public NodeInput2D,
	virtual protected JEventCallback
{
public:
	// Necessary to keep track of past input events for click, double click, etc.
	virtual void update(float tpf);
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event, float tpf);
	
	virtual void callback(InputEvent *event, float tpf){};
	
	// Abstract Callback Methods
	// Note: More than one of these might be called if they overlap
	
	// Only called upon first Press
	virtual void onLeftPress(MouseButtonEvent *event, float tpf) = 0;
	virtual void onRightPress(MouseButtonEvent *event, float tpf) = 0;
	virtual void onMiddlePress(MouseButtonEvent *event, float tpf) = 0;
	
	// Called on Release
	virtual void onLeftRelease(MouseButtonEvent *event, float tpf) = 0;
	virtual void onRightRelease(MouseButtonEvent *event, float tpf) = 0;
	virtual void onMiddleRelease(MouseButtonEvent *event, float tpf) = 0;
	
	// Press and Release happened on the same button
	virtual void onLeftClick(MouseButtonEvent *event, float tpf) = 0;
	virtual void onRightClick(MouseButtonEvent *event, float tpf) = 0;
	virtual void onMiddleClick(MouseButtonEvent *event, float tpf) = 0;
	
	// Double-Click; also generages Single Click
	virtual void onLeftDoubleClick(MouseButtonEvent *event, float tpf) = 0;
	virtual void onRightDoubleClick(MouseButtonEvent *event, float tpf) = 0;
	virtual void onMiddleDoubleClick(MouseButtonEvent *event, float tpf) = 0;
	
protected:
	virtual void precallback(InputEvent *event, float tpf);
};





#endif
