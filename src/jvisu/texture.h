/**
 * Definitions and Declarations of jvisu texture objects.
 * 
 * Some Notes about Memory Management of Textures:
 * 1) Some objects (like Sprites, Animations, etc.) can "own" textures.  These
 *    inherit from the abstract TextureOwner class.
 * 2) A Texture object may be "owned" by multiple TextureOwner objects.
 * 3) Textures maintain a list of all TextureOwner objects which reference them.
 * 4) A texture which is created, but never given to any TextureOwner will never
 *    delete itself.  In such cases, it is up to the user to delete it manually.
 * 5) When a TextureOwner object is deleted, it is removed from the reference
 *    lists of all Texture objects it owns.  If that list is empty, the texture
 *    automatically frees itself.  Thus, all textures given to a TextureOwner
 *    will eventually be deleted, when their last owner is deleted.
 * 6) For memory purposes, a user may manually free a texture.  When this happens,
 *    it is automatically removed from all TextureOwner objects which own it.
 * 7) Also for memory purposes, a user may "unload" a Texture, freeing the bulk
 *    of the memory which it holds while preserving the Texture object itself.
 * 8) An unloaded Texture maintains all of its owners, but will show as a 
 *    transparent rectangle in any viewer.
 * 9) An unloaded Texture will not be reloaded automatically when it is needed;
 *    the user must manually reload so by calling its load method.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <list>

#include "shared_exports.h"
#include "sdl.h"



class TextureOwner;



class SHARED_EXPORT Texture {
public:
	static Texture *createSolidColor(int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	static Texture *loadFromBmp(std::string filepath);
	static Texture *loadFromPng(std::string filepath);
	
	
	const int width, height;
	
	virtual ~Texture();
	
	virtual void load() = 0;
	virtual void unload();
	bool isLoaded() const;
	
	void addOwner(TextureOwner *owner);
	void removeOwner(TextureOwner *owner);
	
	SDL_Texture *getSdlTexture() const;
	
protected:
	bool loaded;
	SDL_Texture *sdlTexture;
	
	Texture(int w, int h);

private:
	std::list<TextureOwner*> owners;
};




class SHARED_EXPORT TextureOwner {
friend class Texture;
public:
	virtual ~TextureOwner(); // Must have destructor which deletes all textures
protected:
	virtual void removeTexture(Texture *texture) = 0;
};



#endif



























