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
	setBackgroundColor(0x00, 0x00, 0x00);
}

LayerBackground::LayerBackground(
	JWindow *win,
	std::string i,
	Uint8 red,
	Uint8 green,
	Uint8 blue
):
	Layer(win, i)
{
	setBackgroundColor(red, green, blue);
}

LayerBackground::LayerBackground(
	JWindow *win,
	std::string i,
	std::string imagePath
):
	Layer(win, i)
{
	setBackgroundColor(0x00, 0x00, 0x00);
	setBackgroundImage(imagePath);
}

LayerBackground::~LayerBackground(){
	if(backgroundImageSurface != NULL){
		SDL_FreeSurface(backgroundImageSurface);
		backgroundImageSurface = NULL;
	}
	if(backgroundColorSurface != NULL){
		SDL_FreeSurface(backgroundColorSurface);
		backgroundColorSurface = NULL;
	}
}


void LayerBackground::render(SDL_Surface *targetSurface){
	/**
	 * Renders the current Layer to the provided surface.
	 */
	if(backgroundColorSurface != NULL){
		SDL_BlitSurface(backgroundColorSurface, NULL, targetSurface, NULL);
	}
	if(backgroundImageSurface != NULL){
		SDL_BlitSurface(backgroundImageSurface, NULL, targetSurface, NULL);
	}
}

void LayerBackground::clearBackgroundImage(){
	if(backgroundImageSurface != NULL){
		SDL_FreeSurface(backgroundImageSurface);
		backgroundImageSurface = NULL;
	}
}

void LayerBackground::setBackgroundImage(std::string imagePath){
	
	SDL_Surface *loadedImage = IMG_Load(imagePath.c_str());
	if(loadedImage == NULL){
		printf("Unable to load background image \"%s\"\n", imagePath.c_str());
		return;
	}
	
	clearBackgroundImage();
	backgroundImageSurface = SDL_ConvertSurface(loadedImage, window->getFormat(), 0);
	
	SDL_FreeSurface(loadedImage);
}

void LayerBackground::setBackgroundColor(Uint8 red, Uint8 green, Uint8 blue){
	backgroundColorRed = red;
	backgroundColorGreen = green;
	backgroundColorBlue = blue;
	
	// Make a new SDL surface for the new color
	Uint32 backgroundColor = SDL_MapRGB(window->getFormat(), red, green, blue);
	if(backgroundColorSurface == NULL){
		backgroundColorSurface = window->createNewSurface();
	}
	SDL_FillRect(backgroundColorSurface, NULL, backgroundColor);
}





































