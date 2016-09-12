#include <cstdlib>
#include <string>
#include <list>

#include "sdl.h"
#include "text.h"
#include "window.h"
#include "renderable.h"
#include "viewport.h"
#include "texture.h"
#include "scene_graph.h"

using namespace ssg;



/*
 * TextObject
 */
TextObject::TextObject():
	text(""),
	fontPath(""),
	fontSize(12),
	colorRed(0xff),
	colorGreen(0xff),
	colorBlue(0xff),
	colorAlpha(0xff),
	oldText(""),
	oldFontPath(""),
	oldFontSize(12)
{
	oldColor.r = 0xff;
	oldColor.g = 0xff;
	oldColor.b = 0xff;
	oldColor.a = 0xff;
}



/*
 * ComponentSpriteText2D
 */

ComponentSpriteText2D::ComponentSpriteText2D(Window *win):
	window(win)
{}


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
		Window *win = layer->getWindow();
		if(win == NULL) return;
		
		
		float targetRatio = width / height;
		float sourceRatio = texture->getAspectRatio();
		
		
		float pixelRatio;
		Vector2f offset;
		if(sourceRatio > targetRatio){
			pixelRatio = width / texture->width;
			offset.set(0, -0.5f * (height - texture->height * pixelRatio));
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
	
}


/*
 * ComponentTextBox2D
 */

ComponentTextBox2D::ComponentTextBox2D(Window *win):
	lineCount(1),
	spacingRatio(1.0f),
	window(win),
	oldLineCount(0),
	oldSpacingRatio(1.0f),
	oldWidth(-1.0f),
	oldHeight(-1.0f)
{
	refreshTextures();
}


ComponentTextBox2D::~ComponentTextBox2D(){
	while(!lineList.empty()){
		delete lineList.front();
		lineList.pop_front();
	}
}




void ComponentTextBox2D::update(Layer2D *layer, float tpf){
	Component2D::update(layer, tpf);
	
	// Refresh line textures, if needed
	refreshTextures();
	
	std::list<ComponentSpriteText2D*>::iterator iter;
	for(iter = lineList.begin(); iter != lineList.end(); iter++){
		ComponentSpriteText2D *line = *iter;
		if(line != NULL) line->update(layer, tpf);
	}
}



void ComponentTextBox2D::collectRenderables(
	std::list<Renderable*> &render_list,
	Viewport2D &viewport,
	float zmod
){
	if(isHidden()) return;
	
	// Do nothing if no dimensions are set
	if(width < 0 || height < 0) return;
	
	
	// Collect renderables from line compoments
	std::list<ComponentSpriteText2D*>::iterator iter;
	for(iter = lineList.begin(); iter != lineList.end(); iter++){
		ComponentSpriteText2D *line = *iter;
		if(line != NULL) line->collectRenderables(render_list, viewport, zmod);
	}
}



static float size_text(std::string fontPath, int size, std::string text){
	/**
	 * This is a static helper method for estimating the ratio of the width of
	 * a piece of rendered text to its height.  If the provided font is invalid
	 * or rendering is otherwise not possible, this returns -1.  Note that no
	 * actual rendering is done; just a call to TTF_SizeText();
	 */
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), size);
	if(font == NULL) return -1;
	
	int w, h;
	TTF_SizeText(font, text.c_str(), &w, &h);
	
	TTF_CloseFont(font);
	
	return (float) w / (float) h;
}


void ComponentTextBox2D::refreshTextures(){
	/**
	 * Checks to see if it is necessary to re-render the textures for the
	 * individual lines of the textbox.  If it is necessary, their textures are
	 * recalculated.
	 */
	
	bool needsRefresh = false;
	
	// Check to see if lines need to be added/removed
	if(lineCount != oldLineCount){
		
		while( (signed int) lineList.size() != lineCount ){
			if(oldLineCount < lineCount){
				ComponentSpriteText2D *line = new ComponentSpriteText2D(window);
				line->parent = this;
				lineList.push_back(line);
			}else if( !lineList.empty() ){
				ComponentSpriteText2D *line = lineList.back();
				lineList.pop_back();
				if( line != NULL ) delete line;
			}else{
				break;
			}
		}
		
		oldLineCount = lineCount;
		needsRefresh = true;
	}
	
	if(
		text.compare(oldText) != 0 ||
		fontPath.compare(oldFontPath) != 0 ||
		fontSize != oldFontSize ||
		colorRed != oldColor.r ||
		colorGreen != oldColor.g ||
		colorBlue != oldColor.b ||
		colorAlpha != oldColor.a ||
		width != oldWidth ||
		height != oldHeight ||
		spacingRatio != oldSpacingRatio
	){
		needsRefresh = true;
		
		// Update Archived Values
		oldText = text;
		oldFontPath = fontPath;
		oldFontSize = fontSize;
		oldColor.r = colorRed;
		oldColor.g = colorGreen;
		oldColor.b = colorBlue;
		oldColor.a = colorAlpha;
		oldWidth = width;
		oldHeight = height;
		oldSpacingRatio = spacingRatio;
	}
	
	
	if(!needsRefresh) return;
	
	/*
	 * At this point, we know that a refresh is needed.
	 */
	
	// The height of each individual line (not including spacing)
	float lineHeight = height / ((1 + spacingRatio) * (lineCount - 1) + 1);
	
	// Make sure all line sprites have the proper settings
	std::list<ComponentSpriteText2D*>::iterator iter;
	int lineNumber = 0;
	for(iter = lineList.begin(); iter != lineList.end(); iter++){
		ComponentSpriteText2D *line = *iter;
		if(line == NULL) continue;
		
		line->text.clear();
		line->text = " ";
		line->fontPath = fontPath;
		line->fontSize = fontSize;
		line->colorRed = colorRed;
		line->colorGreen = colorGreen;
		line->colorBlue = colorBlue;
		line->colorAlpha = colorAlpha;
		line->width = -1;
		line->height = lineHeight;
		line->position.set(0, -lineNumber * (1 + spacingRatio) * lineHeight);
		
		lineNumber++;
	}
	
	
	/*
	 * Note:
	 * Lest ye be tempted to merge the two loops, remember that the above actions
	 * must be done for all lines, regardless of whether or not they are written
	 * to.
	 */
	
	
	char *buffer = (char*) malloc((text.size() + 1) * sizeof(char));
	strcpy(buffer, text.c_str());
	
	// First token
	char *token = strtok(buffer, " ");
	
	std::string testString;
	
	for(iter = lineList.begin(); iter != lineList.end(); iter++){
		ComponentSpriteText2D *line = *iter;
		if(line == NULL) continue;
		
		
		// First token
		if(token == NULL) break;
		
		line->text.clear();
		testString.clear();
		
		testString.append(" ");
		testString.append(token);
		
		float testLength = 0;
		while(testLength < width / lineHeight && testLength >= 0){
			line->text.append(" ");
			line->text.append(token);
			
			token = strtok(NULL, " ");
			if(token == NULL) break;
			
			testString.append(" ");
			testString.append(token);
			
			// estimate width ratio of test string
			testLength = size_text(fontPath, fontSize, testString);
		}
	}
	
	free(buffer);
}




