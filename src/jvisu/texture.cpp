#include "sdl.h"
#include "texture.h"


Texture::Texture(int w, int h):
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


