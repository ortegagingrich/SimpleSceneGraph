#include <cstdio>
#include <algorithm>
#include <string>

#include "sdl.h"
#include "window.h"
#include "texture.h"

using namespace ssg;


/*
 * Texture Factory Methods
 */

Texture *Texture::createSolidColor(int w, int h, Window *win,
                                   Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	if(win == NULL || w < 1 || h < 1) return NULL;
	
	TextureSolid *texture = new TextureSolid(w, h, win, r, g, b, a);
	texture->load();
	return texture;
}


Texture *Texture::createFromText(
	std::string text,
	std::string font,
	int size,
	Window *win,
	Uint8 r,
	Uint8 g,
	Uint8 b,
	Uint8 a
){
	if(win == NULL) return NULL;
	
	// We need to determine the dimensions of the texture, which requires a pre-rendering
	int w = -1;
	int h = -1;
	
	TTF_Font *fnt = TTF_OpenFont(font.c_str(), size);
	if(fnt == NULL){
		printf("Could not load font: %s\n", TTF_GetError());
		return NULL;
	}
	TTF_SizeText(fnt, text.c_str(), &w, &h);
	if(w == -1 || h == -1){
		printf("Problem with font?\n");
		return NULL;
	}
	TTF_CloseFont(fnt);
	
	
	TextureText *texture = new TextureText(w, h, win, text, font, size, r, g, b, a);
	
	if(texture->load()){
		return texture;
	}else{
		delete texture;
		return NULL;
	}
	
}


Texture *Texture::createFromFile(std::string path, Window *win){
	if(win == NULL) return NULL;
	
	// Crude, but necessary: we need to determine the dimensions of the texture.
	SDL_Surface *test = IMG_Load(path.c_str());
	if(test == NULL){
		printf("Unable to load texture from \"%s\"\n", path.c_str());
		return NULL;
	}
	int w = test->w;
	int h = test->h;
	SDL_FreeSurface(test);
	
	
	TextureImage *texture = new TextureImage(w, h, win, path);
	
	// Make sure the texture loads (i.e. image path is valid)
	if(texture->load()){
		return texture;
	}else{
		delete texture;
		return NULL;
	}
	
}


/*
 * Texture Solid
 */

TextureSolid::TextureSolid(int w, int h, Window *win,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a):
	Texture(w, h, win),
	colorRed(r),
	colorGreen(g),
	colorBlue(b),
	colorAlpha(a)
{}


bool TextureSolid::load(){
	if(isLoaded()) return true;
	
	SDL_Renderer *renderer = window->getRenderer();
	if(renderer == NULL){
		printf("Cannot load Texture; Window has no active renderer.\n");
		return false;
	}
	
	SDL_PixelFormat *pixelFormat = window->getFormat();
	
	// Create and fill a surface with the correct color
	SDL_Surface *surface = SDL_CreateRGBSurface(
		0,
		width,
		height,
		pixelFormat->BitsPerPixel,
		pixelFormat->Rmask,
		pixelFormat->Gmask,
		pixelFormat->Bmask,
		pixelFormat->Amask
	);
	Uint32 color = SDL_MapRGBA(pixelFormat, colorRed, colorGreen, colorBlue, colorAlpha);
	SDL_FillRect(surface, NULL, color);
	
	// Make a texture from the surface
	sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
	
	// Clean Up
	SDL_FreeSurface(surface);
	
	return isLoaded();
}



/*
 * TextureText
 */

TextureText::TextureText(
	int w,
	int h,
	Window *win,
	std::string txt,
	std::string font,
	int size,
	Uint8 red,
	Uint8 green,
	Uint8 blue,
	Uint8 alpha
):
	Texture(w, h, win),
	fontName(font),
	fontSize(size),
	text(txt),
	colorRed(red),
	colorGreen(green),
	colorBlue(blue),
	colorAlpha(alpha)
{}


//TODO: duplicated code smell
bool TextureText::load(){
	if(isLoaded()) return true;
	
	SDL_Renderer *renderer = window->getRenderer();
	if(renderer == NULL){
		printf("Cannot load Texture; Window has no active renderer.\n");
		return false;
	}
	
	SDL_Surface *loadedImage, *convertedImage;
	
	TTF_Font *font = TTF_OpenFont(fontName.c_str(), fontSize);
	if(font == NULL){
		printf("Could not load font: %s\n", TTF_GetError());
		return false;
	}
	
	SDL_Color fontColor;
	fontColor.r = colorRed;
	fontColor.g = colorGreen;
	fontColor.b = colorBlue;
	fontColor.a = colorAlpha;
	
	loadedImage = TTF_RenderText_Solid(font, text.c_str(), fontColor);
	if(loadedImage == NULL){
		printf("Could not load font: %s\n", TTF_GetError());
		TTF_CloseFont(font);
		return false;
	}
	
	
	convertedImage = SDL_ConvertSurface(loadedImage, window->getFormat(), 0);
	
	sdlTexture = SDL_CreateTextureFromSurface(renderer, convertedImage);
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
	
	// Cleanup
	TTF_CloseFont(font);
	SDL_FreeSurface(loadedImage);
	SDL_FreeSurface(convertedImage);
	
	return isLoaded();
}



/*
 * TextureImage
 */

TextureImage::TextureImage(int w, int h, Window *win, std::string path):
	Texture(w, h, win),
	filePath(path)
{}


bool TextureImage::load(){
	if(isLoaded()) return true;
	
	
	SDL_Renderer *renderer = window->getRenderer();
	if(renderer == NULL){
		printf("Cannot load Texture; Window has no active renderer.\n");
		return false;
	}
	
	
	SDL_Surface *loadedImage, *convertedImage;
	
	loadedImage = IMG_Load(filePath.c_str());
	if(loadedImage == NULL){
		printf("Unable to load texture from \"%s\"\n", filePath.c_str());
		return false;
	}
	
	convertedImage = SDL_ConvertSurface(loadedImage, window->getFormat(), 0);
	
	sdlTexture = SDL_CreateTextureFromSurface(renderer, convertedImage);
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
	
	
	// Clean Up
	SDL_FreeSurface(loadedImage);
	SDL_FreeSurface(convertedImage);
	
	return isLoaded();
}




/*
 * Texture Class Member Functions
 */

Texture::Texture(int w, int h, Window *win):
	window(win),
	width(w),
	height(h),
	sdlTexture(NULL)
{}


Texture::~Texture(){
	
	for(const auto &result : ownerTable){
		TextureOwner *owner = (TextureOwner*) result.first;
		if(owner != NULL){
			owner->removeTextureReference(this);
		}
	}
	
	
	unload();
}


bool Texture::isLoaded() const {return sdlTexture != NULL;}

void Texture::unload(){
	if(isLoaded()){
		SDL_DestroyTexture(sdlTexture);
		sdlTexture = NULL;
	}
}

bool Texture::reload(){
	unload();
	return load();
}


void Texture::addOwner(TextureOwner *owner){
	if(owner != NULL){
		if(ownerTable.find(owner) != ownerTable.end()){
			ownerTable[owner]++;
		}else{
			ownerTable[owner] = 1;
		}
	}
}


void Texture::removeOwner(TextureOwner *owner){
	
	
	if(ownerTable.find(owner) != ownerTable.end()){
		ownerTable[owner]--;
		if(ownerTable[owner] < 1) ownerTable.erase(owner);
	}
	
	
	if(ownerTable.empty()) delete this;
}


float Texture::getAspectRatio() const {
	return (float) width / (float) height;
}


SDL_Texture *Texture::getSdlTexture() const {return sdlTexture;}



/*
 * TextureCache
 */

TextureCache::~TextureCache(){
	while(!ownedTextures.empty()){
		Texture *texture = ownedTextures.front();
		if(texture != NULL) texture->removeOwner(this);
		ownedTextures.pop_front();
	}
}


void TextureCache::addTexture(Texture *texture){
	if(texture != NULL){
		texture->addOwner(this);
		ownedTextures.push_back(texture);
	}
}


void TextureCache::removeTexture(Texture *texture){
	removeTextureReference(texture);
	if(texture != NULL) texture->removeOwner(this);
}


void TextureCache::removeTextureReference(Texture *texture){
	ownedTextures.remove(texture);
}


int TextureCache::textureCount(){
	return ownedTextures.size();
}


