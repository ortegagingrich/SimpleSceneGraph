/**
 * Include file for window/viewport options
 */
#ifndef WINDOW_H
#define WINDOW_H

#include "shared_exports.h"

#include <list>
#include <string>
#include "sdl.h"
#include "layer.h"
#include "callback.h"


struct TickRecord;
class CallbackManager;
class JEventCallback;
class Vector2f;

/*
 * Main window class
 */
class SHARED_EXPORT JWindow {
public:
	const bool hardwareAccelerated;
	
	TopCallbackManager callbackManager;
	
	JWindow(int sx, int sy, bool ha);
	~JWindow();
	int activate();
	int dispose();
	
	
	void update(float tpf);
	float tick(int target_fps);
	float getFPS() const; // Only works if tick is being used.
	
	
	bool isActive() const;
	int getScreenWidth() const;
	int getScreenHeight() const;
	float getAspectRatio() const;
	
	// Coordinate Transformations
	void viewportToScreen(float xin, float yin, int &xout, int &yout) const;
	void screenToViewport(int xin, int yin, float &xout, float &yout) const;
	
	
	SDL_PixelFormat *getFormat() const;
	SDL_Renderer *getRenderer();
	SDL_Surface *createNewSurface();
	
	
	void addLayerTop(Layer *layer);
	void addLayerBottom(Layer *layer);
	void addLayerAt(Layer *layer, int position);
	void removeLayer(Layer *layer);
	Layer *getLayerById(std::string id);
	std::list<Layer*> getLayers();
	
	// Input
	bool isKeyPressed(SDL_Keycode keycode);
	bool isLeftMouseButtonPressed();
	bool isRightMouseButtonPressed();
	bool isMiddleMouseButtonPressed();
	int getMouseX();
	int getMouseY();
	
	// Mainly public for testing purposes:
	void processEvent(InputEvent *event, float tpf);
	void processEvent(SDL_Event event, float tpf);
	
private:
	SDL_Window *window;
	std::string windowName;
	
	int screenWidth, screenHeight;
	bool active;
	
	std::list<Layer*> layers;
	
	SDL_Renderer *renderer;
	
	struct TickRecord *tickRecord;
	
	
	bool registerLayer(Layer *layer);
	
	void refresh();
	void processInput(float tpf);
	
};


#endif
