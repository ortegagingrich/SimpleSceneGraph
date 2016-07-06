/*
 * Declarations of public-facing scene graph objects
 */
#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "shared_exports.h"

#include <list>
#include "vectormath.h"
#include "callback.h"


class Layer2D;
class Renderable;
class InputEvent;


class Node2D;

class SHARED_EXPORT Component2D {
	/**
	 * Abstract base class of 2D components.
	 */
public:
	Node2D *parent;
	
	Vector2f position;
	float zLevel;
	float rotation;
	Vector2f scale;
	
	bool inheritPosition;
	bool inheritZLevel;
	bool inheritRotation;
	bool inheritScale;
	bool inheritHidden;
	
	Component2D();
	virtual ~Component2D(); // Detaches itself from the parent first
	
	virtual void collectRenderables(std::list<Renderable*> &render_list);
	virtual void processEvent(InputEvent *event){};
	
	bool isHidden(); // Depends also on the parent
	void hide();
	void show();
	
	Node2D *getParent(){return parent;};
	void detachFromParent();
	
protected:
	
	Vector2f positionAbsolute;
	float zLevelAbsolute;
	float rotationAbsolute;
	Vector2f scaleAbsolute;
	
private:
	bool hidden;
};


class SHARED_EXPORT ComponentInput2D : virtual public Component2D {
	/**
	 * 2D Components which can have input callbacks registered to them
	 */
public:
	CallbackManager callbackManager;
	
	virtual void processEvent(InputEvent *event);
};


class SHARED_EXPORT Node2D : virtual public Component2D {
	/**
	 * 2D Components which can have child components attached to them
	 */
public:
	Node2D();
	virtual ~Node2D(); // Deletes all child components
	
	void attachChild(Component2D *child); // Also detaches child from its current parent
	void detachChild(Component2D *child);
	std::list<Component2D*> getChildren() {return children;}

	virtual void collectRenderables(std::list<Renderable*> &render_list);
	virtual void processEvent(InputEvent *event);
private:
	std::list<Component2D*> children;
};


class SHARED_EXPORT NodeInput2D : public Node2D, public ComponentInput2D {
public:
	virtual void processEvent(InputEvent *event);
};


#endif


