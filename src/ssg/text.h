#ifndef TEXT_H
#define TEXT_H

#include "shared_exports.h"

#include <list>
#include <string>

#include "sdl.h"

#include "scene_graph.h"



namespace ssg {

	class Renderable;
	class Viewport2D;
	class Window;



	/*
	 * Superclass of objects which can hold text
	 */

	class SHARED_EXPORT TextObject {
	public:
	
		// All API
		std::string text;
		std::string fontPath;
		int fontSize;
	
		Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
		TextObject();
	
	protected:
		// Not API
		std::string oldText;
		std::string oldFontPath;
		int oldFontSize;
		SDL_Color oldColor;
	};




	/*
	 * Class of single-line text sprites
	 */

	class SHARED_EXPORT ComponentSpriteText2D: 
		public ComponentSpriteSimple2D,
		public TextObject
	{
	friend class ComponentTextBox2D;
	public:
	
		ComponentSpriteText2D(Window *win);
		
	internal:
		virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
		virtual void collectRenderables(std::list<Renderable*> &r, Viewport2D &v, float zm);
	
	protected:
		Window *window;
	};




	class SHARED_EXPORT ComponentTextBox2D:
		public Component2D,
		public TextObject
	{
	public:
	
		bool fixedSize;
		float width, height;  // Either world or viewport, depending on fixedSize
	
	
		// Number of lines of text to display
		int lineCount;
	
		/*
		 * Ratio of height of space between lines to the height of the lines
		 * themselves.  spaceingRatio = 1 for double spacing, for example.
		 */
		float spacingRatio;
	
	
		ComponentTextBox2D(Window *win);
		~ComponentTextBox2D();
	
	
	internal:
		virtual void update(Layer2D *layer, float tpf);
		
		// Should not be necessary
		virtual void collectRenderables(
			std::list<Renderable*> &render_list,
			Viewport2D &viewport
		){
			collectRenderables(render_list, viewport, 0.0f);
		}
		
		virtual void collectRenderables(
			std::list<Renderable*> &render_list,
			Viewport2D &viewport,
			float zmod
		);
	
	protected:
		Window *window;
	
	private:
		int oldLineCount;
		float oldSpacingRatio, oldWidth, oldHeight;
		/*
		 * List of text sprites to serve as the lines.  If lineCount changes, members
		 * are added and removed to this list.  Note that these lines are also child
		 * components, so they are automatically de-allocated when the textbox is
		 * deleted.
		 */
		std::list<ComponentSpriteText2D*> lineList;
	
		void refreshTextures();
	};

}


#endif
