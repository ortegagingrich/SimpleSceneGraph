/*
 * Declarations of public-facing scene graph objects
 */
#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "shared_exports.h"

#include <string>
#include <list>
#include "texture.h"
#include "vectormath.h"
#include "callback.h"
#include "sdl.h"


class Layer2D;
class Viewport2D;
class Renderable;
class RenderableSprite;
class InputEvent;
class ComponentButtonSimple2D;



class SHARED_EXPORT Component2D {
friend class EventCallback;
friend class Node2D;
friend class ComponentLine2D;
friend class ComponentSpriteSimple2D;
friend class ComponentButtonSimple2D;
friend class ComponentTextBox2D;
	/**
	 * Abstract base class of 2D components.
	 */
public:
	
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
	
	
	// Not API
	virtual void update(Layer2D *layer, float tpf);
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v) = 0;
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
	
	
	// API
	bool isHidden(); // Depends also on the parent
	void hide();
	void show();
	void toggleVisibility();
	
	
	// Not part of API
	virtual Layer2D *getLayer();
	Component2D *getParent();
	void detachFromParent();
	
	// These are necessary, but not part of API
	virtual void detachChild(Component2D *child){};
	virtual void deleteAllChildren(){};
	virtual bool isNode(){ return false; };
	virtual bool isVirtual(){ return false; };
	
	
	// Not API
	Vector2f computeRelativePosition(Vector2f worldCoordinates);
	
protected:
	
	// Not API
	Vector2f positionAbsolute;
	float zLevelAbsolute;
	float rotationAbsolute;
	Vector2f scaleAbsolute;
	
	void computeAbsolutePosition(Component2D *reference);
	
private:
	Component2D *parent;
	
	CallbackManager callbackManager;
	
	bool hidden;
};



class SHARED_EXPORT ComponentPoint2D : public Component2D {
	/**
	 * 2D components which are rendered as a single point
	 * TODO: For now, only fixed radius; Future: world radius
	 */
public:
	Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	ComponentPoint2D();
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
};



class SHARED_EXPORT ComponentLine2D : public Component2D {
	/**
	 * 2D components which are rendered as a single line
	 * TODO: For now, just a fixed width (pixels); Future: with that changes with zooming
	 */
public:
	Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	Vector2f startCoordinates, endCoordinates;
	
	ComponentLine2D();
	ComponentLine2D(float x1, float y1, float x2, float y2);
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
private:
	// For internal use only; these are not really components
	ComponentPoint2D start, end;
};



class SHARED_EXPORT ComponentSpriteSimple2D:
	public Component2D,
	public TextureOwner
{
friend class ComponentButtonSimple2D;
public:
	bool fixedSize;
	
	float width, height;  // Either world or viewport, depending on fixedSize
	// Offset of the center of rotation from the upper-left corner of the rectangle.
	Vector2f centerOffset;
	
	ComponentSpriteSimple2D();
	ComponentSpriteSimple2D(Texture *tex);
	virtual ~ComponentSpriteSimple2D();
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void collectRenderables(std::list<Renderable*> &r, Viewport2D &v, float zmod);
	
	Texture *getTexture() const;
	void setTexture(Texture *tex);
	
	
	virtual void removeTexture(Texture *tex);
	
protected:
	RenderableSprite *makeRenderableFromTexture(Texture *tex, Viewport2D &viewport);
private:
	Texture *texture;
	// For internal use
	ComponentPoint2D corner;
};







//TODO: Separate Node into "public" node and "private" node

class SHARED_EXPORT Node2D : public Component2D {
	/**
	 * 2D Components which can have child components attached to them
	 */
public:
	Node2D();
	virtual ~Node2D(); // Deletes all child components
	
	void attachChild(Component2D *child); // Also detaches child from its current parent
	virtual void detachChild(Component2D *child);
	virtual void deleteAllChildren();
	std::list<Component2D*> getChildren() {return children;};
	
	virtual bool isNode(){ return true; };
	
	
	virtual void update(Layer2D *layer, float tpf);
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
	
protected:
	void updateChildren(Layer2D *layer, float tpf);
	void collectChildRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
private:
	std::list<Component2D*> children;
};


class SHARED_EXPORT NodeVirtual2D : public Node2D {
public:
	virtual bool isVirtual(){ return true; };
};



class SHARED_EXPORT NodeRoot2D : public Node2D {
public:
	NodeRoot2D(Layer2D *l);
	
	virtual Layer2D *getLayer();
	
private:
	Layer2D *layer;
};





#endif


