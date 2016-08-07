

#include <string>
#include <list>

#include "sdl.h"
#include "text.h"
#include "window.h"
#include "renderable.h"
#include "viewport.h"
#include "texture.h"
#include "scene_graph.h"


/*
 * ComponentSpriteText2D
 */

ComponentSpriteText2D::ComponentSpriteText2D(JWindow *win):
	ComponentSpriteSimple2D(NULL),
	text(""),
	fontPath(""),
	fontSize(12),
	colorRed(0xff),
	colorGreen(0xff),
	colorBlue(0xff),
	colorAlpha(0xff),
	window(win),
	oldText(""),
	oldFontPath(""),
	oldFontSize(12)
{
	oldColor.r = 0xff;
	oldColor.g = 0xff;
	oldColor.b = 0xff;
	oldColor.a = 0xff;
}


void ComponentSpriteText2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport
){
	collectRenderables(render_list, viewport, 0.0f);
}


void ComponentSpriteText2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport,
	float zmod
){
	// Check to see if we need to replace the texture
	if(
		text.compare(oldText) != 0 ||
		fontPath.compare(oldFontPath) != 0 ||
		fontSize != oldFontSize ||
		colorRed != oldColor.r ||
		colorGreen != oldColor.g ||
		colorBlue != oldColor.b ||
		colorAlpha != oldColor.a
	){
		Texture *newTexture = Texture::createFromText(
			text,
			fontPath,
			fontSize,
			window,
			colorRed,
			colorGreen,
			colorBlue,
			colorAlpha
		);
		setTexture(newTexture);
		
		if(newTexture != NULL){
			float ratio = (float) newTexture->width / (float) newTexture->height;
			width = height * ratio;
		}
		
		// Update Archived Values
		oldText = text;
		oldFontPath = fontPath;
		oldFontSize = fontSize;
		oldColor.r = colorRed;
		oldColor.g = colorGreen;
		oldColor.b = colorBlue;
		oldColor.a = colorAlpha;
	}
	
	// Call super method for collecting renderables
	ComponentSpriteSimple2D::collectRenderables(render_list, viewport, zmod);
}


