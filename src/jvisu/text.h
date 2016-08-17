#ifndef TEXT_H
#define TEXT_H

#include "shared_exports.h"

#include <list>
#include <string>

#include "sdl.h"

#include "scene_graph.h"


class Renderable;
class Viewport2D;
class JWindow;



class SHARED_EXPORT ComponentSpriteText2D : public ComponentSpriteSimple2D {
friend class ComponentTextBox2D;
public:
	
	std::string text;
	std::string fontPath;
	int fontSize;
	
	Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	
	ComponentSpriteText2D(JWindow *win);
	
	virtual void collectRenderables(std::list<Renderable*> &render_list, Viewport2D &v);
	virtual void collectRenderables(std::list<Renderable*> &r, Viewport2D &v, float zm);
	
protected:
	JWindow *window;
	
private:
	std::string oldText;
	std::string oldFontPath;
	int oldFontSize;
	SDL_Color oldColor;
};



//TODO: Should not inherit centerOffset (possibly get rid of that, period?)
class SHARED_EXPORT ComponentTextBox2D:
	public ComponentSpriteText2D,
	public Node2D //TODO: change to private node, once implemented
{
public:
	// Number of lines of text to display
	int lineCount;
	
	/*
	 * Ratio of height of space between lines to the height of the lines
	 * themselves.  spaceingRatio = 1 for double spacing, for example.
	 */
	float spacingRatio;
	
	
	ComponentTextBox2D(JWindow *win);
	
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




#endif
