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
	
	class OnClick : public MouseButtonCallback {
	public:
		Layer2D *layer;
		OnClick(Layer2D *l): MouseButtonCallback(l), layer(l) {}
		
		virtual void callback(MouseButtonEvent *event){
			if(event->isReleased() && event->isRightButton()){
				Vector2f wc = event->getWorldCoordinates(layer);
				
				printf("Right click at: [WORLD] (%f, %f), ", wc.x, wc.y);
				printf("[SCREEN] (%d, %d)\n", event->screenX, event->screenY);
			}
		}
	};
	new OnClick(layer2d);
	
	
	/*
	 * Quit Callback
	 */
	class OnExit : public QuitEventCallback {
	public:
		OnExit(JWindow *win): QuitEventCallback(win), window(win) {};
		virtual void callback(QuitEvent *event){
			printf("Disposing from new Callback.\n");
			window->dispose();
			event->consume();
		};
	private:
		JWindow *window;
	};
	new OnExit(window);
	
	
	
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
