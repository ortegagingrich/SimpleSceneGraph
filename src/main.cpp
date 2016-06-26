#include <cstdio>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "jvisu/jvisu.h"



/*
 * This function is the primary entry point for demo/testing purposes.
 */
int main(int argc, char* argv[]){
	printf("\nEntry Point for jvisu demos.\n");
	
	JWindow *window = new JWindow(1280, 720);
	
	
	/*
	 * Background Handling
	 */
	LayerBackground *background = (LayerBackground*) window->getLayerById("background");
	background->setBackgroundColor(0x55, 0x77, 0xbb);
	
	class LayerController : public JEventCallback {
	public:
		LayerBackground *background;
		
		LayerController(JWindow *window, LayerBackground *bg):
			JEventCallback(window),
			background(bg),
			imageShowing(false) {};
		virtual ~LayerController(){};
		
		virtual void callback(SDL_Event event){
			/*
			 * Keyboard Event (space) for changing the background color
			 */
			if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
				SDL_KeyboardEvent key = event.key;
				if(key.keysym.sym == SDLK_SPACE){
					if(key.state == SDL_PRESSED){
						background->setBackgroundColor(0xa1, 0xaa, 0xdd);
					}else{
						background->setBackgroundColor(0x55, 0x77, 0xbb);
					}
				}
			}
			/*
			 * Mouse Event to toggle the background image
			 */
			if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
				SDL_MouseButtonEvent button = event.button;
				if(button.state == SDL_RELEASED){
					if(button.button == SDL_BUTTON_LEFT){
						if(imageShowing){
							background->clearBackgroundImage();
							imageShowing = false;
						}else{
							background->setBackgroundImage("assets/test/wood.bmp");
							imageShowing = true;
						}
					}
				}
				printf("MouseButtonEvent at (%d, %d).\n", button.x, button.y);
			}
		}
	private:
		bool imageShowing;
	};
	new LayerController(window, background);
	
	
	/*
	 * Try to make a quit callback function
	 */
	class QuitCallback : public JEventCallback {
	public:
		QuitCallback(JWindow *window): JEventCallback(window) {};
		virtual ~QuitCallback(){};
		virtual void callback(SDL_Event event){
			if(event.type != SDL_QUIT) return;
			printf("Disposing from Callback.\n");
			boundWindow->dispose();
		};
	};
	new QuitCallback(window);
	
	
	// Main Loop
	while(window->isActive()){
		
		window->update();
		
		//Sleep the appropriate amount of time for the frame
#ifdef __linux__
		usleep(16*1000);
#endif
#ifdef _WIN32
		Sleep(16);
#endif
	}

	
	delete window;
	
	printf("Process Exiting.\n");
	
	return 0;
}
