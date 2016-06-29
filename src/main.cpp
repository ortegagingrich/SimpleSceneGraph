#include <cstdio>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "jvisu/jvisu.h"


#ifdef __linux__
const bool USE_HARDWARE_ACCELERATION = false;
#endif
#ifdef _WIN32
const bool USE_HARDWARE_ACCELERATION = true;
#endif


/*
 * This function is the primary entry point for demo/testing purposes.
 */
int main(int argc, char* argv[]){
	printf("\nEntry Point for jvisu demos.\n");
	
	JWindow *window = new JWindow(1280, 720, USE_HARDWARE_ACCELERATION);
	
	
	/*
	 * Background Handling
	 */
	LayerBackground *background = (LayerBackground*) window->getLayerById("background");
	background->setBackgroundColor(0x55, 0x77, 0xbb, 0x00);
	
	class LayerController : public JEventCallback {
	public:
		LayerBackground *background;
		
		LayerController(LayerBackground *bg):
			JEventCallback(bg),
			background(bg) {};
		virtual ~LayerController(){};
		
		virtual void callback(InputEvent *jevent){
			//Temporary Work-around:
			SDL_Event event = jevent->sdlEvent;
			
			/*
			 * Keyboard Event (space) for changing the background color
			 */
			if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
				SDL_KeyboardEvent key = event.key;
				if(key.keysym.sym == SDLK_SPACE){
					if(key.state == SDL_PRESSED){
						background->setBackgroundColor(0xa1, 0xaa, 0xdd, 0x00);
					}else{
						background->setBackgroundColor(0x55, 0x77, 0xbb, 0x00);
					}
				}
				
			}
			/*
			 * Mouse Event to toggle the background image
			 */
			if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
				SDL_MouseButtonEvent button = event.button;
				if(button.state == SDL_RELEASED || button.state == SDL_PRESSED){
					if(button.button == SDL_BUTTON_LEFT){
						Uint8 alpha = (Uint8) button.y % 255;
						background->clearBackgroundImage();
						background->setBackgroundImage("assets/test/cracks.png", alpha);
					}
				}
			}
		}
	};
	LayerController *lc = new LayerController(background);
	//delete lc;
	
	
	/*
	 * Layer2D Test
	 */
	Layer2D *layer2d = new Layer2D("2dlayer");
	Node2D *rootNode = layer2d->rootNode;
	window->addLayerTop(layer2d);
	class CoordinateTest : public JEventCallback {
	public:
		Layer2D *layer;
		CoordinateTest(Layer2D *l): JEventCallback(l), layer(l) {}
		
		virtual void callback(InputEvent *jevent){
			// Temporary work-around
			SDL_Event event = jevent->sdlEvent;
			if(event.type == SDL_MOUSEBUTTONUP){
				SDL_MouseButtonEvent button = event.button;
				if(button.button == SDL_BUTTON_RIGHT){
					// Compute World Coordinates
					float wx, wy;
					jevent->window->screenToViewport(button.x, button.y, wx, wy);
					layer->viewport.viewportToWorld(wx, wy, wx, wy);
					printf("Right click at: [WORLD] (%f, %f), ", wx, wy);
					printf("[SCREEN] (%d, %d)\n", button.x, button.y);
				}
			}
		}
	};
	new CoordinateTest(layer2d);
	
	
	/*
	 * Try to make a quit callback function
	 */
	class QuitCallback : public JEventCallback {
	public:
		QuitCallback(JWindow *win): JEventCallback(win), window(win) {};
		virtual ~QuitCallback(){};
		virtual void callback(InputEvent *jevent){
			SDL_Event event = jevent->sdlEvent;
			
			if(event.type != SDL_QUIT) return;
			printf("Disposing from Callback.\n");
			window->dispose();
			
			jevent->consume();
		};
	private:
		JWindow *window;
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
