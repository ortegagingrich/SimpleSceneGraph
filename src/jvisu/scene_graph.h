/*
 * Declarations of public-facing scene graph objects
 */
#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "shared_exports.h"

#include <list>
#include "vectormath.h"
#include "callback.h"
#include "sdl.h"


class Layer2D;
class Viewport2D;
class Renderable;
class InputEvent;


class Node2D;

class SHARED_EXPORT Component2D {
friend class ComponentLine2D;
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
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event){};
	
	bool isHidden(); // Depends also on the parent
	void hide();
	void show();
	
	Node2D *getParent(){return parent;};
	void detachFromParent();
	
	Vector2f computeRelativePosition(Vector2f worldCoordinates);
	
protected:
	Vector2f positionAbsolute;
	float zLevelAbsolute;
	float rotationAbsolute;
	Vector2f scaleAbsolute;
	
	void computeAbsolutePosition(Component2D *reference);
	
private:
	bool hidden;
};



class SHARED_EXPORT ComponentPoint2D : virtual public Component2D {
	/**
	 * 2D components which are rendered as a single point
	 * TODO: For now, only fixed radius; Future: world radius
	 */
public:
	Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	ComponentPoint2D();
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
};



class SHARED_EXPORT ComponentLine2D : virtual public Component2D {
	/**
	 * 2D components which are rendered as a single line
	 * TODO: For now, just a fixed width (pixels); Future: with that changes with zooming
	 */
public:
	Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	Vector2f startCoordinates, endCoordinates;
	
	ComponentLine2D();
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
private:
	// For internal use only; these are not really components
	ComponentPoint2D start, end;
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
	void deleteAllChildren();
	std::list<Component2D*> getChildren() {return children;}

	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event);
private:
	std::list<Component2D*> children;
};



class SHARED_EXPORT NodeInput2D : public Node2D, public ComponentInput2D {
public:
	virtual void processEvent(InputEvent *event);
};


#endif


