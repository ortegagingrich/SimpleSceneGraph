#include <list>

#include "scene_graph.h"
#include "renderable.h"
#include "vectormath.h"


/*
 * Component2D
 */

Component2D::Component2D():
	position(0,0),
	zlevel(0),
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


void Component2D::render(std::list<Renderable*> render_list){
	/*
	 * This superclass method is primarily responsible for computing the absolute
	 * scale/rotation/position variables based on the parent values.
	 */
	
	scaleAbsolute = scale;
	if(inheritScale && parent != NULL){
		scaleAbsolute.scale(parent->scaleAbsolute);
	}
	
	rotationAbsolute = rotation;
	if(inheritRotation && parent != NULL){
		rotationAbsolute += parent->rotationAbsolute;
	}
	
	//Now for the tricky part: handling position
	positionAbsolute = position;
	if(parent != NULL){
		positionAbsolute.scale(parent->scaleAbsolute); // Scale it
		positionAbsolute.rotate(parent->rotationAbsolute);  // Rotate it
		
		if(inheritPosition){
			positionAbsolute += parent->positionAbsolute;
		}
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
 * Node2D
 */

Node2D::Node2D(): Component2D() {}
Node2D::~Node2D(){
	// Delete (and thereby detach) all child components
	while(!children.empty()){
		delete children.front();
	}
}

void Node2D::render(std::list<Renderable*> render_list){
	// First render the node itself with the super method
	Component2D::render(render_list);
	
	// Render all child components
	std::list<Component2D*>::iterator iter;
	for(iter = children.begin(); iter != children.end(); iter++){
		Component2D *child = *iter;
		child->render(render_list);
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


