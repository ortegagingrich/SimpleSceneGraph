#include "sdl.h"
#include "window.h"
#include "texture.h"


/*
 * Texture Factory Methods
 */

Texture *Texture::createSolidColor(int w, int h, JWindow *win,
                                   Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	if(win == NULL) return NULL;
	TextureSolid *texture = new TextureSolid(w, h, win, r, g, b, a);
	texture->load();
	return texture;
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


void TextureSolid::load(){
	unload(); // just in case it is already loaded
	
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
	sdlTexture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
		
	// Clean Up
	SDL_FreeSurface(surface);
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


void Texture::addOwner(TextureOwner *owner){
	if(owner != NULL) owners.push_front(owner);
}


void Texture::removeOwner(TextureOwner *owner){
	owners.remove(owner);
	if(owners.empty()) delete this;
}


SDL_Texture *Texture::getSdlTexture() const {return sdlTexture;}


