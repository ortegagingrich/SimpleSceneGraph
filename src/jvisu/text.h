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



class SHARED_EXPORT ComponentTextBox2D : public ComponentSpriteText2D {
public:
	// Number of lines of text to display
	int lineCount;
	
	/*
	 * Ratio of height of space between lines to the height of the lines
	 * themselves.  spaceingRatio = 1 for double spacing, for example.
	 */
	float spacingRatio;
	
	
	ComponentTextBox2D(JWindow *win);
	
	virtual void collectRenderables(
		std::list<Renderable*> &render_list,
		Viewport2D &v,
		float zmod
	);
	
}




#endif
