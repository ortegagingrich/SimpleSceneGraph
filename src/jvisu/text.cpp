

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
	
	
	/*
	 * Instead of calling the sprite super method, we handle things directly here,
	 * though a bit differently.
	 */
	
	Component2D::collectRenderables(render_list, viewport);
	if(isHidden()) return;
	
	RenderableSprite *sprite = NULL;
	
	
	Texture *texture = getTexture();
	if(texture == NULL) return;
	
	/*
	 * Special Handling for selected Rectangles
	 */
	if(width > 0 && height > 0){
		Layer2D *layer = getLayer();
		if(layer == NULL) return;
		JWindow *window = layer->getWindow();
		if(window == NULL) return;
		
		
		float targetRatio = width / height;
		float sourceRatio = texture->getAspectRatio();
		
		
		float pixelRatio;
		Vector2f offset;
		if(sourceRatio > targetRatio){
			pixelRatio = width / texture->width;
			offset.set(0, 0.5f * (height - texture->height * pixelRatio));
		}else{
			pixelRatio = height / texture->height;
			offset.set(0.5f * (width - texture->width * pixelRatio), 0);
		}
		
		offset.add(-centerOffset.x, centerOffset.y);
		offset.rotate(rotationAbsolute);
		
		
		Vector2f vc;
		float scaleFactorX = scaleAbsolute.x;
		float scaleFactorY = scaleAbsolute.y;
		
		if(fixedSize){
			vc = viewport.worldToViewport(positionAbsolute) + offset;
		}else{
			vc = viewport.worldToViewport(positionAbsolute + offset);
			scaleFactorX *= viewport.getInverseRadiusY();
			scaleFactorY *= viewport.getInverseRadiusY();
		}
		
		
		// Compute base width and height of text rectangle in viewport coordinates.
		float w = pixelRatio * texture->width;
		float h = pixelRatio * texture->height;
		
		
		// Finally, make the renderable
		sprite = RenderableSprite::createRenderableSprite(
			vc.x,
			vc.y,
			scaleFactorX * w,
			scaleFactorY * h,
			zLevel,
			rotationAbsolute,
			texture,
			viewport.getViewportRect()
		);
		
	}else{
		sprite = makeRenderableFromTexture(texture, viewport);
	}
	
	
	
	
	
	
	
	if(sprite != NULL){
		sprite->zMod = zmod;
		render_list.push_back(sprite);
	}
	
	
	// Call super method for collecting renderables
	//ComponentSpriteSimple2D::collectRenderables(render_list, viewport, zmod);
}


