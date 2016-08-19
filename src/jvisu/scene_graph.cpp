#include <cstdio>
#include <list>

#include "scene_graph.h"
#include "renderable.h"
#include "texture.h"
#include "vectormath.h"
#include "input.h"
#include "callback.h"
#include "viewport.h"
#include "window.h"


/*
 * Component2D
 */

Component2D::Component2D():
	position(0,0),
	zLevel(0),
	rotation(0),
	scale(1,1),
	inheritPosition(true),
	inheritZLevel(true),
	inheritRotation(true),
	inheritScale(true),
	inheritHidden(true),
	positionAbsolute(0,0),
	zLevelAbsolute(0),
	rotationAbsolute(0),
	scaleAbsolute(1,1),
	parent(NULL),
	hidden(false)
{}

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
	// Set default values
	zLevelAbsolute = zLevel;
	scaleAbsolute = scale;
	rotationAbsolute = rotation;
	positionAbsolute = position;
	
	if(reference == NULL) return;
	
	if(inheritZLevel){
		zLevelAbsolute += reference->zLevelAbsolute;
	}
	
	if(inheritScale){
		scaleAbsolute.scale(reference->scaleAbsolute);
	}
	
	if(inheritRotation){
		rotationAbsolute += reference->rotationAbsolute;
	}
	
	//Now for the tricky part: handling position
	if(inheritPosition){
		positionAbsolute.scale(reference->scaleAbsolute); // Scale it
		positionAbsolute.rotate(reference->rotationAbsolute);  // Rotate it
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

void Component2D::toggleVisibility(){
	hidden = !hidden;
}

void Component2D::detachFromParent(){
	if(parent != NULL){
		parent->detachChild(this);
	}
}


Layer2D *Component2D::getLayer(){
	if(parent != NULL){
		return parent->getLayer();
	}else{
		return NULL;
	}
}


Vector2f Component2D::computeRelativePosition(Vector2f worldCoordinates){
	/**
	 * Computes the position of the provided coordinates relative to this
	 * component.  Note that this assumes that this component has non-zero
	 * absolute scale factors.  If that is not so, the result may contain non-
	 * numerical values.
	 */
	// Just to make sure everything is up to date
	computeAbsolutePosition(parent);
	
	// Translate
	Vector2f relativeCoordinates = worldCoordinates - positionAbsolute;
	
	// Rotate
	relativeCoordinates %= -rotationAbsolute;
	
	// Scale
	relativeCoordinates.scale(1.0 / scaleAbsolute.x, 1.0 / scaleAbsolute.y);
	
	return relativeCoordinates;
}


/*
 * ComponentPoint2D
 */

ComponentPoint2D::ComponentPoint2D():
	colorRed(0xff),
	colorGreen(0xff),
	colorBlue(0xff),
	colorAlpha(0xff)
{}


void ComponentPoint2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	Component2D::collectRenderables(render_list, v);
	if(isHidden()) return;
	
	// Get Viewport Coordinates
	Vector2f vc = v.worldToViewport(positionAbsolute);
	
	// Create renderable
	RenderablePoint *point = NULL;
	point = RenderablePoint::createRenderablePoint(
		vc.x,
		vc.y,
		zLevelAbsolute,
		1,
		colorRed,
		colorGreen,
		colorBlue,
		colorAlpha,
		v.getViewportRect()
	);
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
	colorAlpha(0xff)
{}


ComponentLine2D::ComponentLine2D(float x1, float y1, float x2, float y2):
	colorRed(0xff),
	colorGreen(0xff),
	colorBlue(0xff),
	colorAlpha(0xff)
{
	startCoordinates.set(x1, y1);
	endCoordinates.set(x2, y2);
}



void ComponentLine2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	Component2D::collectRenderables(render_list, v);
	if(isHidden()) return;
	
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
	vc1 = v.worldToViewport(start.positionAbsolute);
	vc2 = v.worldToViewport(end.positionAbsolute);
	
	// Finally, make the renderable
	RenderableLine *line;
	line = RenderableLine::createRenderableLine(
		vc1.x,
		vc1.y,
		vc2.x,
		vc2.y,
		zLevelAbsolute,
		1,
		colorRed,
		colorGreen,
		colorBlue,
		colorAlpha,
		v.getViewportRect()
	);
	if(line != NULL){
		render_list.push_back(line);
	}
}



/*
 * ComponentSpriteSimple2D
 */

ComponentSpriteSimple2D::ComponentSpriteSimple2D():
	fixedSize(false),
	width(0.1),
	height(0.1),
	centerOffset(0, 0),
	texture(NULL)
{}


ComponentSpriteSimple2D::ComponentSpriteSimple2D(Texture *tex):
	fixedSize(false),
	width(0.1),
	height(0.1),
	centerOffset(0, 0),
	texture(NULL)
{
	setTexture(tex);
}


ComponentSpriteSimple2D::~ComponentSpriteSimple2D(){
	if(texture != NULL){
		texture->removeOwner(this);
	}
}


void ComponentSpriteSimple2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport
){
	collectRenderables(render_list, viewport, 0.0f);
}


void ComponentSpriteSimple2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport,
	float zmod
){
	Component2D::collectRenderables(render_list, viewport);
	if(isHidden()) return;
	
	RenderableSprite *sprite = makeRenderableFromTexture(texture, viewport);
	
	if(sprite != NULL){
		sprite->zMod = zmod;
		render_list.push_back(sprite);
	}
}


RenderableSprite *ComponentSpriteSimple2D::makeRenderableFromTexture(
	Texture *tex,
	Viewport2D &viewport
){
	
	/*
	 * This Method is a complicated mess.  Three different nearly-unrelated
	 * cases are treated with the same code, hence the unreadability.
	 * TODO: Reorganize this and add liberal comment descriptions of what is being
	 * done and for what cases.
	 */
	
	if(tex == NULL) return NULL;
	
	Layer2D *layer = getLayer();
	if(layer == NULL) return NULL;
	JWindow *window = layer->getWindow();
	if(window == NULL) return NULL;
	
	
	bool fixedPixel = false;
	float w = width, h = height;
	if(width < 0){
		if(height < 0){
			w = tex->width;
			h = tex->height;
			fixedPixel = true;
		}else{
			w = h * tex->getAspectRatio();
		}
	}else if(height < 0){
		h = w / tex->getAspectRatio();
	}
	
	
	
	Vector2f offset(-centerOffset.x, centerOffset.y);
	if(fixedPixel){
		offset.scale(2.0f / window->getScreenHeight());
		if(!fixedSize){
			offset.scale(viewport.getInverseRadiusY());
		}
	}
	offset.scale(scaleAbsolute.x, scaleAbsolute.y);
	offset.rotate(rotationAbsolute);
	
	
	float scaleFactorX = scaleAbsolute.x;
	float scaleFactorY = scaleAbsolute.y;
	
	Vector2f vc;
	if(!fixedSize){
		vc = viewport.worldToViewport(positionAbsolute + offset);
		if(!fixedPixel){
			scaleFactorX *= viewport.getInverseRadiusY();
			scaleFactorY *= viewport.getInverseRadiusY();	
		}
	}else{
		// Fixed Size Sprites
		vc = viewport.worldToViewport(positionAbsolute) + offset;
	}
	
	
	if(fixedPixel){
		int pixelFactor = window->getScreenHeight() / 2;
		scaleFactorX /= pixelFactor;
		scaleFactorY /= pixelFactor;
	}
	
	
	// Finally make the renderable
	RenderableSprite *sprite;
	sprite = RenderableSprite::createRenderableSprite(
		vc.x,
		vc.y,
		scaleFactorX * w,
		scaleFactorY * h,
		zLevel,
		rotationAbsolute,
		tex,
		viewport.getViewportRect()
	);
	
	
	return sprite;
	
}



Texture *ComponentSpriteSimple2D::getTexture() const {return texture;}

void ComponentSpriteSimple2D::setTexture(Texture *tex){
	if(texture != NULL) texture->removeOwner(this);
	if(tex != NULL) tex->addOwner(this);
	texture = tex;
}

// Internal Use Only!  Called when a texture is manually deleted on its owners
void ComponentSpriteSimple2D::removeTexture(Texture *tex){
	if(texture == tex) texture = NULL;
}



/*
 * ComponentImage2D
 */

ComponentImage2D::ComponentImage2D(Texture *tex):
	offsetX(0),
	offsetY(0),
	width(tex->width),
	height(tex->height),
	texture(NULL)
{
	setTexture(tex);
}

ComponentImage2D::ComponentImage2D(Texture *tex, int offset_x, int offset_y):
	offsetX(offset_x),
	offsetY(offset_y),
	width(tex->width),
	height(tex->height),
	texture(NULL)
{
	setTexture(tex);
}


ComponentImage2D::~ComponentImage2D(){
	if(texture != NULL) texture->removeOwner(this);
}


void ComponentImage2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport
){
	Component2D::collectRenderables(render_list, viewport);
	if(isHidden()) return;
	
	if(texture == NULL) return;
	
	
	Vector2f vc = viewport.worldToViewport(positionAbsolute);
	
	
	
	RenderableSpriteFixed *sprite = NULL;
	sprite = RenderableSpriteFixed::createRenderableSpriteFixed(
		vc.x,
		vc.y,
		offsetX,
		offsetY,
		zLevel,
		texture,
		viewport.getViewportRect()
	);
	
	if(sprite != NULL){
		render_list.push_back(sprite);
	}
}


Texture *ComponentImage2D::getTexture() const {return texture;}

void ComponentImage2D::setTexture(Texture *tex){
	if(texture != NULL) texture->removeOwner(this);
	if(tex != NULL) tex->addOwner(this);
	texture = tex;
}

void ComponentImage2D::removeTexture(Texture *tex){
	if(texture == tex) texture = NULL;
}




/*
 * ComponentInput2D
 */

void ComponentInput2D::processEvent(InputEvent *event, Layer2D *layer, float tpf){
	Component2D::processEvent(event, layer, tpf);
	
	callbackManager.processEvent(event, tpf);
}


/*
 * Node2D
 */

Node2D::Node2D(): Component2D() {}
Node2D::~Node2D(){
	deleteAllChildren();
}


void Node2D::update(Layer2D *layer, float tpf){
	// Update all children
	std::list<Component2D*>::iterator iter;
	for(iter = children.begin(); iter != children.end(); iter++){
		Component2D *child = *iter;
		child->update(layer, tpf);
	}
}


void Node2D::collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v){
	// First collect renderables for the node itself with the super method
	Component2D::collectRenderables(render_list, v);
	
	collectChildRenderables(render_list, v);
}

void Node2D::collectChildRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport
){
	// Collect renderables for all child components
	std::list<Component2D*>::iterator iter;
	for(iter = children.begin(); iter != children.end(); iter++){
		Component2D *child = *iter;
		child->collectRenderables(render_list, viewport);
	}
}

void Node2D::processEvent(InputEvent *event, Layer2D *layer, float tpf){
	// The node itself does nothing; the event is passed on to children
	std::list<Component2D*>::iterator iter;
	for(iter = children.begin(); iter != children.end(); iter++){
		Component2D *child = *iter;
		child->processEvent(event, layer, tpf);
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

void Node2D::deleteAllChildren(){
	/**
	 * Not only detaches, but also deletes all child components.
	 */
	while(!children.empty()){
		delete children.front();
	}
}


/*
 * NodeRoot2D
 */

NodeRoot2D::NodeRoot2D(Layer2D *l): layer(l) {}

Layer2D *NodeRoot2D::getLayer(){
	return layer;
}


/*
 * NodeInput2D
 */

void NodeInput2D::processEvent(InputEvent *event, Layer2D *layer, float tpf){
	ComponentInput2D::processEvent(event, layer, tpf);
	Node2D::processEvent(event, layer, tpf);
}


