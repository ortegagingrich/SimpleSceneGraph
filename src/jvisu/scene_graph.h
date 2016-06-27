/*
 * Declarations of public-facing scene graph objects
 */
#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <list>
#include "vectormath.h"

class Renderable;


class Node2D;

class Component2D {
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


class Node2D : public Component2D {
	/**
	 * 2D Components which can have child components attached to it
	 */
public:
	Node2D();
	virtual ~Node2D(); // Deletes all child components
	
	virtual void collectRenderables(std::list<Renderable*> &render_list);
	
	void attachChild(Component2D *child); // Also detaches child from its current parent
	void detachChild(Component2D *child);
	std::list<Component2D*> getChildren() {return children;}
	
private:
	std::list<Component2D*> children;
};


#endif


