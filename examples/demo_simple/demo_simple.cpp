/**
 * A simple demo demonstrating some input callbacks and background layers.
 */

#include <cstdio>
#include <cmath>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "jvisu.h"


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
	printf("\nSimple Demo for jvisu.\n");
	printf("\tPress Space to toggle background color.\n");
	printf("\tLeft Click to toggle background image.\n");
	printf("\tRight Click to print both screen and \"world\" coordinates.\n");
	
	JWindow *window = new JWindow(1280, 720, USE_HARDWARE_ACCELERATION);
	
	
	/*
	 * Background Handling
	 */
	LayerBackground *background = (LayerBackground*) window->getLayerById("background");
	background->setBackgroundColor(0x55, 0x77, 0xbb, 0xff);
	
	
	class OnLeftClick : public MouseButtonCallback {
	public:
		LayerBackground *background;
		
		OnLeftClick(LayerBackground *bg): MouseButtonCallback(bg), background(bg) {}
		
		virtual void callback(MouseButtonEvent *event, float tpf){
			if(event->isLeftButton()){
				Vector2f vc = event->getViewportCoordinates();
				Uint8 alpha = (Uint8) 30 + std::abs(vc.norm() * 140);
				background->clearBackgroundImage();
				background->setBackgroundImage("assets/test/cracks.png", alpha);
			}
		}
	};
	new OnLeftClick(background);
	
	
	class OnSpaceBar : public KeyButtonCallback {
	public:
		LayerBackground *background;
		
		OnSpaceBar(LayerBackground *bg): KeyButtonCallback(bg), background(bg) {}
		
		virtual void callback(KeyButtonEvent *event, float tpf){
			if(event->key == SDLK_SPACE){
				if(event->isPressed()){
					background->setBackgroundColor(0xa1, 0xaa, 0xdd, 0xff);
				}else{
					background->setBackgroundColor(0x55, 0x77, 0xbb, 0xff);
				}
			}
		}
	};
	new OnSpaceBar(background);
	
	
	
	/*
	 * Layer2D Test
	 */
	Layer2D *layer2d = new Layer2D("2dlayer");
	Node2D *rootNode = layer2d->getRootNode();
	window->addLayerTop(layer2d);
	
	class OnRightClick : public MouseButtonCallback {
	public:
		Layer2D *layer;
		OnRightClick(Layer2D *l): MouseButtonCallback(l), layer(l) {}
		
		virtual void callback(MouseButtonEvent *event, float tpf){
			if(event->isReleased() && event->isRightButton()){
				Vector2f wc = event->getWorldCoordinates(layer);
				
				printf("Right click at: [WORLD] (%f, %f), ", wc.x, wc.y);
				printf("[SCREEN] (%d, %d)\n", event->screenX, event->screenY);
			}
		}
	};
	new OnRightClick(layer2d);
	
	
	/*
	 * Quit Callback
	 */
	class OnExit : public QuitEventCallback {
	public:
		OnExit(JWindow *win): QuitEventCallback(win), window(win) {};
		virtual void callback(QuitEvent *event, float tpf){
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
		
		float tpf = window->tick(60);
		
		window->update(tpf);
		
	}

	
	delete window;
	
	printf("Process Exiting.\n");
	
	return 0;
}
