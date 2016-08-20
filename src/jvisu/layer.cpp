#include <cstdio>

#include "sdl.h"
#include "window.h"
#include "layer.h"
#include "scene_graph.h"
#include "button.h"
#include "viewport.h"
#include "renderable.h"
#include "input.h"



/*
 * Source for Abstract Base Layer Class
 */

Layer::Layer(std::string i): id(i), window(NULL) {}

Layer::~Layer(){
	if(window == NULL) return;
	// Remove this layer from the window
	window->removeLayer(this);
}


void Layer::processEvent(InputEvent *event, float tpf){
	callbackManager.processEvent(event, tpf);
}


JWindow *Layer::getWindow(){
	return window;
}

void Layer::setWindow(JWindow *win){
	/**
	 * Should only be called by the window.
	 */
	// If already in a window, remove it
	if(window != NULL){
		window->removeLayer(this);
	}
	
	window = win;
}



/*
 * Source for Layer2D
 */

Layer2D::Layer2D(std::string i): Layer(i) {
	rootNode = new NodeRoot2D(this);
}

Layer2D::~Layer2D(){
	if(rootNode != NULL) delete rootNode;
}


void Layer2D::update(float tpf){
	// Update the scene graph starting with the root node
	if(rootNode != NULL) rootNode->update(this, tpf);
}


void Layer2D::render(SDL_Renderer *renderer){
	if(window == NULL){
		printf("Cannot render layer \"%s\"; ", id.c_str());
		printf("it is currently associated with any window.\n");
	}
	
	// Make sure that the viewport matches the aspect ratio of the window
	if(viewport.isAspectRatioLocked()){
		viewport.forceAspectRatio(window->getAspectRatio());
	}
	
	if(rootNode == NULL){
		printf("Cannot Render layer \"%s\"; rootNode is NULL.\n", id.c_str());
		return;
	}
	
	// Construct list of rendererables by recursively traversing the scene graph
	renderables.clear();
	rootNode->collectRenderables(renderables, viewport);
	
	// Sort the render list by z value
	sort_renderables_by_z_level(renderables);
	
	// Render the renderables in z-order
	while(!renderables.empty()){
		Renderable *renderable = renderables.front();
		if(renderable != NULL){
			renderable->render(renderer, window);
			delete renderable;
		}
		renderables.pop_front();
	}
}

void Layer2D::processEvent(InputEvent *event, float tpf){
	// Pass the event down the scene graph
	rootNode->processEvent(event, this, tpf);
	
	// Process any buttons which were activated in the scene graph
	buttonManager.processEvent(event, tpf);
	
	// Call the super method to process the event with this layer
	Layer::processEvent(event, tpf);
}


Node2D *Layer2D::getRootNode(){
	return rootNode;
}


/*
 * Source for LayerBackground
 */

LayerBackground::LayerBackground(std::string i):
	Layer(i)
{
	setBackgroundColor(0x00, 0x00, 0x00, 0xff);
}

LayerBackground::LayerBackground(
	std::string i,
	Uint8 red,
	Uint8 green,
	Uint8 blue,
	Uint8 alpha
):
	Layer(i)
{
	setBackgroundColor(red, green, blue, alpha);
}

LayerBackground::LayerBackground(
	std::string i,
	std::string imagePath
):
	Layer(i)
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
		SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
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
	if(window == NULL){
		printf("Cannot set background image; layer not attached to a window.\n");
		return;
	}
	
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


