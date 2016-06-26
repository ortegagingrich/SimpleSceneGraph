/*
 * Definitions and Declarations for renderable objects.
 * These are small lightweight objects which are created by some Layer sub-types
 * at render time.
 */
#import <list>
#import "sdl.h"


class Renderable {
	/**
	 * Abstract base class of renderables
	 */
public:
	const float zLevel;
	Renderable(float z): zLevel(z) {}
	virtual void render(SDL_Renderer *renderer) = 0;
}





/*
 * Helper Functions
 */

void sort_renderables_by_z_level(std::list<Renderable*> &renderables){
	/**
	 * Sorts the provided list of renderables by their zLevel values
	 */
	bool compare_zlevel(const Renderable* &a, const Renderable* &b){
		return a->zLevel < b->zLevel;
	}
	
	renderables.sort(compare_zlevel);
}

