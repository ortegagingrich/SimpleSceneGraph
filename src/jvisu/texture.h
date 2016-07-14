/**
 * Definitions and Declarations of jvisu texture objects.
 * 
 * Some Notes about Memory Management of Textures:
 * 1) Some objects (like Sprites, Animations, etc.) can "own" textures.  These
 *    inherit from the abstract TextureOwner class.
 * 2) A Texture object may be "owned" by multiple TextureOwner objects.
 * 3) Textures maintain a list of all TextureOwner objects which reference them.
 * 4) When a TextureOwner object is deleted, it is removed from the reference
 *    lists of all Texture objects it owns.  If that list is empty, the texture
 *    automatically frees itself.
 * 5) For memory purposes, a user may manually free a texture.  When this happens,
 *    it is automatically removed from all TextureOwner objects which own it.
 * 6) Also for memory purposes, a user may "unload" a Texture, freeing the bulk
 *    of the memory which it holds while preserving the Texture object itself.
 * 7) An unloaded Texture maintains all of its owners, but will show as a 
 *    transparent rectangle in any viewer.
 * 8) An unloaded Texture will not be reloaded automatically when it is needed;
 *    the user must manually do so by calling its load method.
 * 9) A texture which is created, but never given to any TextureOwner will never
 *    delete itself.  In such cases, it is up to the user to delete it manually.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <list>











#endif

