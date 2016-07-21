#include <cstdio>
#include <string>

#include "sdl.h"
#include "window.h"
#include "texture.h"


/*
 * Texture Factory Methods
 */

Texture *Texture::createSolidColor(int w, int h, JWindow *win,
                                   Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	if(win == NULL || w < 1 || h < 1) return NULL;
	
	TextureSolid *texture = new TextureSolid(w, h, win, r, g, b, a);
	texture->load();
	return texture;
}


Texture *Texture::createFromFile(std::string path, JWindow *win){
	if(win == NULL) return NULL;
	
	// Crude, but necessary: we need to determin the dimensions of the texture.
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

TextureSolid::TextureSolid(int w, int h, JWindow *win,
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
 * TextureImage
 */

TextureImage::TextureImage(int w, int h, JWindow *win, std::string path):
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

Texture::Texture(int w, int h, JWindow *win):
	window(win),
	width(w),
	height(h),
	sdlTexture(NULL)
{}


Texture::~Texture(){
	
	// Detach the texture from all of its parents
	while(!owners.empty()){
		TextureOwner *owner = owners.front();
		owner->removeTexture(this);
		owners.pop_front();
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
	if(owner != NULL) owners.push_front(owner);
}


void Texture::removeOwner(TextureOwner *owner){
	owners.remove(owner);
	if(owners.empty()) delete this;
}


SDL_Texture *Texture::getSdlTexture() const {return sdlTexture;}


