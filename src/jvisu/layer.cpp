#include "sdl.h"
#include "window.h"
#include "scene_graph.h"



/*
 * Source for Abstract Base Layer Class
 */

Layer::Layer(JWindow *win, std::string i): window(win), id(i) {}

Layer::~Layer(){
	// Remove this layer from the window
	window->removeLayer(this);
}


/*
 * Source for LayerBackground
 */

LayerBackground::LayerBackground(
	JWindow *win,
	std::string i
):
	Layer(win, i)
{
	setBackgroundColor(0x00, 0x00, 0x00, 0xff);
}

LayerBackground::LayerBackground(
	JWindow *win,
	std::string i,
	Uint8 red,
	Uint8 green,
	Uint8 blue,
	Uint8 alpha
):
	Layer(win, i)
{
	setBackgroundColor(red, green, blue, alpha);
}

LayerBackground::LayerBackground(
	JWindow *win,
	std::string i,
	std::string imagePath
):
	Layer(win, i)
{
	setBackgroundColor(0x00, 0x00, 0x00, 0xff);
	setBackgroundImage(imagePath, 0x00);
}

LayerBackground::~LayerBackground(){
	if(backgroundImageSurface != NULL){
		SDL_FreeSurface(backgroundImageSurface);
		backgroundImageSurface = NULL;
	}
}


void LayerBackground::render(SDL_Renderer *renderer){
	/**
	 * Renders the current Layer to the provided surface.
	 */
	
	// Render the background color
	SDL_SetRenderDrawColor(
		renderer,
		backgroundColorRed,
		backgroundColorGreen,
		backgroundColorBlue,
		backgroundColorAlpha
	);
	SDL_RenderFillRect(renderer, NULL);
	
	
	// Now render the image
	
	if(backgroundImageSurface != NULL){
		SDL_Texture *image = SDL_CreateTextureFromSurface(renderer, backgroundImageSurface);
		SDL_SetTextureAlphaMod(image, backgroundImageAlpha);
		SDL_RenderCopy(renderer, image, NULL, NULL);
		SDL_DestroyTexture(image);
	}
}

void LayerBackground::clearBackgroundImage(){
	if(backgroundImageSurface != NULL){
		SDL_FreeSurface(backgroundImageSurface);
		backgroundImageSurface = NULL;
	}
}

void LayerBackground::setBackgroundImage(std::string imagePath, Uint8 alpha){
	
	backgroundImageAlpha = alpha;
	
	// First load the PNG into a software surface
	SDL_Surface *loadedImage = IMG_Load(imagePath.c_str());
	if(loadedImage == NULL){
		printf("Unable to load background image \"%s\"\n", imagePath.c_str());
		return;
	}
	
	clearBackgroundImage();
	backgroundImageSurface = SDL_ConvertSurface(loadedImage, window->getFormat(), 0);
	
	SDL_FreeSurface(loadedImage);
}

void LayerBackground::setBackgroundColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
	backgroundColorRed = red;
	backgroundColorGreen = green;
	backgroundColorBlue = blue;
	backgroundColorAlpha = alpha;
}

