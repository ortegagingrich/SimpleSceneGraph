#include <list>

#include "scene_graph.h"
#include "renderable.h"
#include "vectormath.h"
#include "input.h"
#include "callback.h"
#include "viewport.h"


/*
 * Component2D
 */

Component2D::Component2D():
	parent(NULL),
	position(0,0),
	zLevel(0),
	rotation(0),
	scale(1,1),
	inheritPosition(true),
	inheritZLevel(true),
	inheritRotation(true),
	inheritScale(true),
	inheritHidden(true),
	hidden(false){}

Component2D::~Component2D(){
	detachFromParent();
}


void Component2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	/*
	 * This superclass method is primarily responsible for computing the absolute
	 * scale/rotation/position variables based on the parent values.
	 */
	
	computeAbsolutePosition(parent);
}


void Component2D::computeAbsolutePosition(Component2D *reference){
	if(reference == NULL) return;
	
	zLevelAbsolute = zLevel;
	if(inheritZLevel){
		zLevelAbsolute += reference->zLevelAbsolute;
	}
	
	scaleAbsolute = scale;
	if(inheritScale){
		scaleAbsolute.scale(reference->scaleAbsolute);
	}
	
	rotationAbsolute = rotation;
	if(inheritRotation){
		rotationAbsolute += reference->rotationAbsolute;
	}
	
	//Now for the tricky part: handling position
	positionAbsolute = position;
	positionAbsolute.scale(reference->scaleAbsolute); // Scale it
	positionAbsolute.rotate(reference->rotationAbsolute);  // Rotate it
	
	if(inheritPosition){
		positionAbsolute += reference->positionAbsolute;
	}
}


bool Component2D::isHidden(){
	if(inheritHidden && parent != NULL){
		return hidden || parent->isHidden();
	}else{
		return hidden;
	}
}

void Component2D::hide(){
	hidden = true;
}

void Component2D::show(){
	hidden = false;
}

void Component2D::detachFromParent(){
	if(parent != NULL){
		parent->detachChild(this);
	}
}


/*
 * ComponentPoint2D
 */

ComponentPoint2D::ComponentPoint2D():
	colorRed(0xff),
	colorGreen(0xff),
	colorBlue(0xff),
	colorAlpha(0x00)
{}


void ComponentPoint2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	Component2D::collectRenderables(render_list, v);
	
	// Get Viewport Coordinates
	Vector2f vc = v.worldToViewport(positionAbsolute);
	
	// Create renderable
	RenderablePoint *point = NULL;
	point = RenderablePoint::createRenderablePoint(vc.x, vc.y, zLevelAbsolute, 1,
	                                colorRed, colorGreen, colorBlue, colorAlpha);
	if(point != NULL){
		render_list.push_back(point);
	}
}


/*
 * ComponentLine2D
 */

ComponentLine2D::ComponentLine2D():
	colorRed(0xff),
	colorGreen(0xff),
	colorBlue(0xff),
	colorAlpha(0x00)
{}


void ComponentLine2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	Component2D::collectRenderables(render_list, v);
	
	/*
	 * Here, we need to compute the positions of the dummy endpoint components.
	 */
	start.position = startCoordinates;
	end.position = endCoordinates;
	start.computeAbsolutePosition(this);
	end.computeAbsolutePosition(this);
	
	/*
	 * Now we can get the viewport coordinates of the endpoints.
	 */
	Vector2f vc1, vc2;
	vc1 = start.positionAbsolute;
	vc2 = end.positionAbsolute;
	
	// Finally, make the renderable
	RenderableLine *line;
	line = RenderableLine::createRenderableLine(vc1.x, vc1.y, vc2.x, vc2.y,
	            zLevelAbsolute, 1, colorRed, colorGreen, colorBlue, colorAlpha);
	if(line != NULL){
		render_list.push_back(line);
	}
}



/*
 * ComponentInput2D
 */

void ComponentInput2D::processEvent(InputEvent *event){
	Component2D::processEvent(event);
	
	callbackManager.processEvent(event);
}


/*
 * Node2D
 */

Node2D::Node2D(): Component2D() {}
Node2D::~Node2D(){
	// Delete (and thereby detach) all child components
	while(!children.empty()){
		delete children.front();
	}
}

void Node2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	// First collect renderables for the node itself with the super method
	Component2D::collectRenderables(render_list, v);
	
	// Collect renderables for all child components
	std::list<Component2D*>::iterator iter;
	for(iter = children.begin(); iter != children.end(); iter++){
		Component2D *child = *iter;
		child->collectRenderables(render_list, v);
	}
}

void Node2D::processEvent(InputEvent *event){
	// The node itself does nothing; the event is passed on to children
	std::list<Component2D*>::iterator iter;
	for(iter = children.begin(); iter != children.end(); iter++){
		Component2D *child = *iter;
		child->processEvent(event);
	}
}

void Node2D::attachChild(Component2D *child){
	/**
	 * Provided that it is not null, the provided component is attached to this
	 * node as a child.  If that component is already attached to a node, it is
	 * first detached.
	 */
	if(child == NULL) return;
	
	if(child->parent != NULL){
		child->detachFromParent();
	}
	
	child->parent = this;
	children.push_back(child);
}

void Node2D::detachChild(Component2D *child){
	/**
	 * Detaches the provided component, provided that it is one this node's
	 * children
	 */
	if(child->parent != this) return;
	
	children.remove(child);
	child->parent = NULL;
}


/*
 * NodeInput2D
 */

void NodeInput2D::processEvent(InputEvent *event){
	ComponentInput2D::processEvent(event);
	Node2D::processEvent(event);
}


