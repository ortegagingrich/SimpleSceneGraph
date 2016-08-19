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


class Node2D;

class SHARED_EXPORT Component2D {
friend class Node2D;
friend class ComponentLine2D;
friend class ComponentSpriteSimple2D;
friend class ComponentButtonSimple2D;
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
	
	virtual void update(Layer2D *layer, float tpf){};
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf){};
	
	bool isHidden(); // Depends also on the parent
	void hide();
	void show();
	void toggleVisibility();
	
	virtual Layer2D *getLayer();
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
	Node2D *parent;
	
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
	ComponentLine2D(float x1, float y1, float x2, float y2);
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
private:
	// For internal use only; these are not really components
	ComponentPoint2D start, end;
};



class SHARED_EXPORT ComponentSpriteSimple2D:
	virtual public Component2D,
	public TextureOwner
{
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
	
protected:
	RenderableSprite *makeRenderableFromTexture(Texture *tex, Viewport2D &viewport);
	
	virtual void removeTexture(Texture *tex);
private:
	Texture *texture;
	// For internal use
	ComponentPoint2D corner;
};



//TODO: Deprecated
class SHARED_EXPORT ComponentImage2D:
	virtual public Component2D,
	public TextureOwner
{
public:
	/*
	 * Offset of the upper-left corner of the image in pixels from the actual
	 * pixel position of the component
	 */ 
	const int offsetX, offsetY;
	
	const int width, height; // Locked to those of the texture
	
	ComponentImage2D(Texture *tex);
	ComponentImage2D(Texture *tex, int offset_x, int offset_y);
	virtual ~ComponentImage2D();
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	
	Texture *getTexture() const;
	
protected:
	Texture *texture;
	
	void setTexture(Texture *tex);
	virtual void removeTexture(Texture *tex);
};



//TODO: Figure out a better way to deal with input callbacks
class SHARED_EXPORT ComponentInput2D : virtual public Component2D {
	/**
	 * 2D Components which can have input callbacks registered to them
	 */
public:
	CallbackManager callbackManager;
	
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
};



//TODO: Separate Node into "public" node and "private" node

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
	std::list<Component2D*> getChildren() {return children;};
	
	virtual void update(Layer2D *layer, float tpf);
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
protected:
	void collectChildRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
private:
	std::list<Component2D*> children;
};


class SHARED_EXPORT NodeRoot2D : virtual public Node2D {
public:
	NodeRoot2D(Layer2D *l);
	
	virtual Layer2D *getLayer();
	
private:
	Layer2D *layer;
};



class SHARED_EXPORT NodeInput2D :
	virtual public Node2D,
	virtual public ComponentInput2D {

public:
	virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
};


#endif


