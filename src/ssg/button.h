#ifndef BUTTON_H
#define BUTTON_H

#include <list>

#include "shared_exports.h"

#include "callback.h"
#include "scene_graph.h"
#include "button_manager.h"
#include "vectormath.h"

namespace ssg {
	
	
	class Layer2D;
	class Viewport2D;
	class Texture;
	class TextureOwner;

	class ComponentButtonSimple2D;
	class ComponentSpriteText2D;



	class SHARED_EXPORT ComponentButton2D:
		public Component2D,
		internal EventCallback
	{
	friend class ButtonManager;
	friend class ComponentButtonSimple2D;
	friend class ComponentDraggable2D;
	public:
	
		ComponentButton2D();
		
		
		// Necessary to keep track of past input events for click, double click, etc.
	internal:
		virtual void update(Layer2D *layer, float tpf);
		virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);
		
		virtual void callback(InputEvent *event, float tpf){};
		
	public:
		virtual bool isPressed() const;
	
		// Abstract Callback Methods
		// Note: More than one of these might be called if they overlap
	
		// Only called upon first Press
		virtual void onLeftPress(MouseButtonEvent *event, float tpf){};
		virtual void onRightPress(MouseButtonEvent *event, float tpf){};
		virtual void onMiddlePress(MouseButtonEvent *event, float tpf){};
	
		// Called on Release
		virtual void onLeftRelease(MouseButtonEvent *event, float tpf){};
		virtual void onRightRelease(MouseButtonEvent *event, float tpf){};
		virtual void onMiddleRelease(MouseButtonEvent *event, float tpf){};
	
		// Press and Release happened on the same button
		virtual void onLeftClick(MouseButtonEvent *event, float tpf){};
		virtual void onRightClick(MouseButtonEvent *event, float tpf){};
		virtual void onMiddleClick(MouseButtonEvent *event, float tpf){};
	
		// Double-Click; also generages Single Click
		virtual void onLeftDoubleClick(MouseButtonEvent *event, float tpf){};
		virtual void onRightDoubleClick(MouseButtonEvent *event, float tpf){};
		virtual void onMiddleDoubleClick(MouseButtonEvent *event, float tpf){};
	
		// Mouseover; Only activated the first frame the mouse is over the button
		virtual void onStartMouseOver(MouseMotionEvent *event, float tpf){};
		virtual void onEndMouseOver(MouseMotionEvent *event, float tpf){};
	
	protected:
		virtual bool isInside(float x, float y, Layer2D *layer) = 0;
		virtual bool isInside(Vector2f vc, Layer2D *layer){
			return isInside(vc.x, vc.y, layer);
		};
	
	
		virtual void precallback(InputEvent *event, float tpf);
	
		// Internal Pre-callback functions.
		virtual void preLeftPress(MouseButtonEvent *event, float tpf);
		virtual void preRightPress(MouseButtonEvent *event, float tpf);
		virtual void preMiddlePress(MouseButtonEvent *event, float tpf);
	
		virtual void preLeftRelease(MouseButtonEvent *event, float tpf);
		virtual void preRightRelease(MouseButtonEvent *event, float tpf);
		virtual void preMiddleRelease(MouseButtonEvent *event, float tpf);
	
		virtual void preLeftClick(MouseButtonEvent *event, float tpf);
		virtual void preRightClick(MouseButtonEvent *event, float tpf);
		virtual void preMiddleClick(MouseButtonEvent *event, float tpf);
	
		virtual void preLeftDoubleClick(MouseButtonEvent *event, float tpf);
		virtual void preRightDoubleClick(MouseButtonEvent *event, float tpf);
		virtual void preMiddleDoubleClick(MouseButtonEvent *event, float tpf);
	
		virtual void preStartMouseOver(MouseMotionEvent *event, float tpf);
		virtual void preEndMouseOver(MouseMotionEvent *event, float tpf);
	
	
	private:
		bool pendingLeftClick, pendingRightClick, pendingMiddleClick;
		bool mouseAlreadyOver;
	};






	class SHARED_EXPORT ComponentButtonSimple2D:
		public ComponentButton2D,
		public TextureOwner
	{
	public:
	
		// parallel parameters for mainSprite; same as in ComponentSpriteSimple2D
		bool fixedSize;
		float width, height;
		Vector2f centerOffset;
	
	
	
		ComponentButtonSimple2D(Window *win);
		~ComponentButtonSimple2D();
	
	
		// Node-like methods
		int attachChild(Component2D *child);
		virtual int detachChild(Component2D *child);
		virtual int deleteAllChildren();
		std::list<Component2D*> getChildren();
	
	
		// Main Sprite methods
		Texture *getTexture() const;
		void setTexture(Texture *tex);
	
	
		// Additional Texture Methods
		Texture *getOverlayTexture() const;
		Texture *getPressedTexture() const;
		void setOverlayTexture(Texture *tex);
		void setPressedTexture(Texture *tex);
		
	protected:
		virtual void removeTextureReference(Texture *tex);
	
	public:
		
		// Text Overlay Methods
		void setText(std::string text);
		void clearText();
		void setFont(std::string font);
		void setFontSize(int size);
		void setTextColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	
	
	internal:
		virtual void update(Layer2D *layer, float tpf);
		virtual void collectRenderables(std::list<Renderable*> &r, Viewport2D &v);
		virtual void processEvent(InputEvent *event, Layer2D *layer, float tpf);

	protected:
		virtual bool isInside(float x, float y, Layer2D *layer);

	private:
		Texture *overlayTexture, *pressedTexture;
	
		ComponentSpriteSimple2D *mainSprite;
		ComponentSpriteText2D *textOverlay;
		NodeVirtual2D *virtualNode;
	};




	class SHARED_EXPORT ComponentDraggable2D:
		public ComponentButtonSimple2D
	{
	public:
	
		ComponentDraggable2D(Window *win);
	
		virtual void onDragStart(float tpf){};
		virtual void onDragEnd(float tpf){};
		
	internal:
		virtual void update(Layer2D *layer, float tpf);
	
	protected:
		virtual void preLeftPress(MouseButtonEvent *event, float tpf);
		virtual void preDragStart(float tpf);
		virtual void preDragEnd(float tpf);
	
	private:
		Vector2f anchor;
	};

}

#endif
