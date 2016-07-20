/*
 * SDL stuff
 */
#include <cstdio>
#include <cmath>
#include "sdl.h"
#include "vectormath.h"
#include "geometry.h"


// To keep track of how many windows are active.  If this drops to 0, SDL shuts down.
static int WINDOWS_ACTIVE = 0;
// To keep track of whether or not SDL is presently active.
static bool SDL_ACTIVE = false;


/*
 * Setup/Takedown Methods
 */


static int init(){
	/**
	 * Initializes SDL and returns the result;
	 */
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
	
	// For PNG loading
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return -1;
	
	return 0;
}



int create_SDL_window(SDL_Window **win, const char *window_name, int sx, int sy){
	/**
	 * Attempts to create a new SDL window and place a pointer to it in the
	 * provided address.
	 */
	
	// If necessary, initialize SDL
	if(!SDL_ACTIVE){
		if(init() < 0){
			printf("[Error] Failed to initialize SDL.\n");
			return -1;
		}
		SDL_ACTIVE = true;
	}
	
	// Make a window
	SDL_Window* window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED,
	                     SDL_WINDOWPOS_UNDEFINED, sx, sy, SDL_WINDOW_SHOWN);
	if(window != NULL) WINDOWS_ACTIVE++;
	else{
		printf("[Error] Failed to create SDL Window.\n");
		return -1;
	}
	
	// Fill it with blackness
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	Uint32 backgroundColor = SDL_MapRGB(surface->format, 0x00, 0x00, 0x00);
	SDL_FillRect(surface, NULL, backgroundColor);
	SDL_UpdateWindowSurface(window);
	
	*win = window;
	return 0;
}

void remove_SDL_window(SDL_Window *window){
	/**
	 * Destroys the provided SDL window and shuts down SDL if there are no windows left.
	 */
	if(window == NULL){
		printf("[Error] Cannot destroy NULL window.\n");
		return;
	}
	SDL_DestroyWindow(window);
	WINDOWS_ACTIVE--;
	if(WINDOWS_ACTIVE <= 0) SDL_Quit();
}


SDL_Renderer *create_SDL_renderer(SDL_Window *window, int hardware_accelerated){
	/**
	 * Attempts to create an SDL renderer.
	 */
	Uint32 flags = hardware_accelerated ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE;
	return SDL_CreateRenderer(window, -1, flags);
}



/*
 * SDL Wrapper methods
 */

int render_copy_clip(
	SDL_Renderer *renderer,
	SDL_Texture *texture,
	const SDL_Rect *srcrect_in,
	const SDL_Rect *dstrect_in,
	double angle
){
	int view_w, view_h, tex_w, tex_h;
	SDL_GetRendererOutputSize(renderer, &view_w, &view_h);
	SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
	
	
	SDL_Rect srcrect, dstrect;
	if(srcrect_in != NULL){
		srcrect = *srcrect_in;
	}else{
		srcrect.x = 0;
		srcrect.y = 0;
		srcrect.w = tex_w;
		srcrect.h = tex_h;
	}
	if(dstrect_in != NULL){
		dstrect = *dstrect_in;
	}else{
		dstrect.x = 0;
		dstrect.y = 0;
		dstrect.w = view_w;
		dstrect.h = view_h;
	}
	
	
	
	// Step 1: Rotate View Rectangle
	Vector2f orig_corner(dstrect.x, dstrect.y);
	Vector2f view_corners[4];
	
	
	for(int i = 0; i < 4; i++){
		Vector2f c(0, 0);
		c.add((i / 2) * view_w, (i % 2) * view_h);
		
		c -= orig_corner;
		c %= -angle * DEG_2_RAD;
		c += orig_corner;
		
		view_corners[i] = c;
	}
	
	// Step 2: Compute bounding rectangle
	Rect2f bound = Rect2f::boundPoints(view_corners, 4);
	
	
	// Step 3: Check to see if dstrect is inside the bounding rect
	Rect2f drect(dstrect.x, dstrect.x + dstrect.w, dstrect.y, dstrect.y + dstrect.h);
	Rect2f out;
	calculate_intersection(bound, drect, out);
	bool needs_clipping = true;
	if(out == drect) needs_clipping = false;
	
	
	if(needs_clipping){
		
		// Step 4: Pre-image of Bounding Rectangle
		int src_orig_x = srcrect.x;
		int src_orig_y = srcrect.y;
		
		int prex, prey, prew, preh, xm, ym;
		float wrat = (float) srcrect.w / (float) dstrect.w;
		float hrat = (float) srcrect.h / (float) dstrect.h;
		
		prex = std::floor((bound.xMin - dstrect.x) * wrat) + src_orig_x;
		prey = std::floor((bound.yMin - dstrect.y) * hrat) + src_orig_y;
		xm = std::ceil((bound.xMax - dstrect.x) * wrat) + src_orig_x;
		ym = std::ceil((bound.yMax - dstrect.y) * hrat) + src_orig_y;
		
		
		// Step 5: Intersect pre-image with original srcrect
		prex = prex > srcrect.x ? prex : srcrect.x;
		prey = prey > srcrect.y ? prey : srcrect.y;
		xm = xm < srcrect.x + srcrect.w ? xm : srcrect.x + srcrect.w;
		ym = ym < srcrect.y + srcrect.h ? ym : srcrect.y + srcrect.h;
		
		
		prew = xm - prex;
		preh = ym - prey;
		
		// If no intersection, then nothing to draw
		if(prew <= 0 || preh <= 0) return 0;
		
		
		// Define new srcrect
		srcrect.x = prex;
		srcrect.y = prey;
		srcrect.w = prew;
		srcrect.h = preh;
		
		
		
		// Step 6: Compute the image of the new srcrect
		dstrect.x = std::floor(((prex - src_orig_x) / wrat) + orig_corner.x);
		dstrect.y = std::floor(((prey - src_orig_y) / hrat) + orig_corner.y);
		xm = std::ceil(((xm - src_orig_x) / wrat) + orig_corner.x);
		ym = std::ceil(((ym - src_orig_y) / hrat) + orig_corner.y);
		dstrect.w = xm - dstrect.x;
		dstrect.h = ym - dstrect.y;
		
		
		// Step 7: Reposition the new dstrect appropriately
		Vector2f new_corner((float) dstrect.x, (float) dstrect.y);
		new_corner -= orig_corner;
		new_corner %= angle * DEG_2_RAD;
		new_corner += orig_corner;
		dstrect.x = new_corner.x;
		dstrect.y = new_corner.y;
		
		// TODO: Post-processing for very small src rects?
	}
	
	
	/*
	 * No Clipping necessary, so just use the SDL function
	 */
	SDL_Point center;
	center.x = 0;
	center.y = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	
	
	//SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	return SDL_RenderCopyEx(renderer, texture, &srcrect, &dstrect, angle, &center, flip);
}



































void print_SDL_error(){
	printf("SDL ERROR: %s\n", SDL_GetError());
}

