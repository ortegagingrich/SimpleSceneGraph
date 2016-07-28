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
class InputEvent;


class Node2D;

class SHARED_EXPORT Component2D {
friend class ComponentLine2D;
friend class ComponentSpriteSimple2D;
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
	virtual void processEvent(InputEvent *event, float tpf){};
	
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
	
	Texture *getTexture() const;
	void setTexture(Texture *tex);
	
protected:
	virtual void removeTexture(Texture *tex);
private:
	Texture *texture;
	// For internal use
	ComponentPoint2D corner;
};



class SHARED_EXPORT ComponentSpriteText2D: public ComponentSpriteSimple2D {
public:
	
	std::string text;
	std::string fontPath;
	int fontSize;
	
	Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	
	ComponentSpriteText2D(JWindow *win);
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	
private:
	JWindow *window;
	
	std::string oldText;
	std::string oldFontPath;
	int oldFontSize;
	SDL_Color oldColor;
};



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



class SHARED_EXPORT ComponentInput2D : virtual public Component2D {
	/**
	 * 2D Components which can have input callbacks registered to them
	 */
public:
	CallbackManager callbackManager;
	
	virtual void processEvent(InputEvent *event, float tpf);
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
	virtual void processEvent(InputEvent *event, float tpf);
protected:
	void collectChildRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
private:
	std::list<Component2D*> children;
};



class SHARED_EXPORT NodeInput2D :
	virtual public Node2D,
	virtual public ComponentInput2D {

public:
	virtual void processEvent(InputEvent *event, float tpf);
};





class SHARED_EXPORT ComponentButtonSimple2D:
	virtual public ComponentSpriteSimple2D,
	virtual public NodeInput2D,
	virtual protected JEventCallback
{
	
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


