#include <cstdio>
#include <cmath>
#include <sstream>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "jvisu.h"


#ifdef __linux__
const bool USE_HARDWARE_ACCELERATION = true;
#endif
#ifdef _WIN32
const bool USE_HARDWARE_ACCELERATION = true;
#endif



/*
 * Class Definitions for this Demo
 */

class FPSCounter : public ComponentSpriteText2D {
public:
	FPSCounter(JWindow *win): ComponentSpriteText2D(win){
		fontPath = "assets/font/LiberationSerif-Regular.ttf";
		fontSize = 24;
		text = "[FPS]";
		
		position = Vector2f(-win->getAspectRatio(), 1.0f);
		height = 0.1f;
	}
	
	virtual void update(Layer2D *layer, float tpf){
		ComponentSpriteText2D::update(layer, tpf);
		
		// Make new FPS string
		std::stringstream stream;
		stream << "FPS: " << window->getFPS();
		text = stream.str();
	}
};



class TestButton : public ComponentButtonSimple2D {
public:
	std::string label;
	JWindow *window;
	
	TestButton(JWindow *win, std::string txt): window(win), label(txt) {
		Texture *base, *overlay, *pressed;
		base = Texture::createFromFile("assets/test/button.png", win);
		overlay = Texture::createSolidColor(1, 1, win, 0xbb, 0x88, 0xbb, 0x55);
		pressed = Texture::createFromFile("assets/test/button_pressed.png", win);
		//pressed = Texture::createSolidColor(1, 1, win, 0x00, 0xff, 0x00, 0x00);
		
		if(pressed == NULL){
			printf("Disaster\n");
			exit(-1);
		}
		
		setTexture(base);
		setOverlayTexture(overlay);
		setPressedTexture(pressed);
	};
	
	/*
	 * Test Callbacks
	 */
	
	virtual void onLeftPress(MouseButtonEvent *event, float tpf){
		printf("[%s] Left Press\n", label.c_str());
	}
	
	virtual void onLeftRelease(MouseButtonEvent *event, float tpf){
		printf("[%s] Left Release\n", label.c_str());
	}
	
	virtual void onLeftClick(MouseButtonEvent *event, float tpf){
		printf("[%s] Left Click\n", label.c_str());
	}
	
	virtual void onStartMouseOver(MouseMotionEvent *event, float tpf){
		printf("[%s] Mouse Over Start\n", label.c_str());
	}
	
	virtual void onEndMouseOver(MouseMotionEvent *event, float tpf){
		printf("[%s] Mouse Over End\n", label.c_str());
	}
	
};





/*
 * This function is the primary entry point for demo/testing purposes.
 */
int main(int argc, char* argv[]){
	printf("\nEntry Point for jvisu demos.\n");
	
	//JWindow *window = new JWindow(1280, 720, USE_HARDWARE_ACCELERATION);
	JWindow *window = new JWindow(1280, 720, USE_HARDWARE_ACCELERATION);
	
	
	
	
	/*
	 * Layer2D Test
	 */
	Layer2D *layer2d = new Layer2D("2dlayer");
	window->addLayerTop(layer2d);
	
	Node2D *mainNode = new Node2D();
	layer2d->rootNode->attachChild(mainNode);
	
	
	/*
	 * Fixed Layer
	 */
	Layer2D *hud = new Layer2D("hud");
	window->addLayerTop(hud);
	hud->rootNode->attachChild(new FPSCounter(window));
	
	TestButton *button1 = new TestButton(window, "Fixed Button");
	hud->rootNode->attachChild(button1);
	
	
	TestButton *button2 = new TestButton(window, "World Button");
	mainNode->attachChild(button2);
	
	
	
	/*
	 * Callback for moving the viewport
	 */
	class ViewportController {
	public:
		const float panSpeed = 0.025;
		const float rotSpeed = 0.05;
		const float zoomFactor = 1.05;
		JWindow *window;
		Layer2D *layer;
		Node2D *mainNode;
		ViewportController(JWindow *win, Layer2D *l, Node2D *m):
			window(win),
			layer(l),
			mainNode(m)
		{}
		
		
		void update(float tpf){
			Vector2f newCenter = layer->viewport.getCenter();
			float vdiff = panSpeed * layer->viewport.getRadiusY();
			float hdiff = panSpeed * layer->viewport.getRadiusY();
			
			// Panning
			
			if(window->isKeyPressed(SDLK_w)){
				newCenter.y += vdiff;
			}
			if(window->isKeyPressed(SDLK_s)){
				newCenter.y -= vdiff;
			}
			if(window->isKeyPressed(SDLK_a)){
				newCenter.x -= hdiff;
			}
			if(window->isKeyPressed(SDLK_d)){
				newCenter.x += hdiff;
			}
			layer->viewport.setCenter(newCenter);
			
			// Rotating
			if(window->isKeyPressed(SDLK_q)){
				mainNode->rotation += rotSpeed;
			}
			if(window->isKeyPressed(SDLK_e)){
				mainNode->rotation -= rotSpeed;
			}
			
			// Zooming
			
			if(window->isKeyPressed(SDLK_p)){
				const float minZoom = 0.0;
				
				float ry = layer->viewport.getRadiusY();
				ry /= zoomFactor;
				if(ry < minZoom) ry = minZoom;
				layer->viewport.setRadiusY(ry);
			}
			if(window->isKeyPressed(SDLK_m)){
				float ry = layer->viewport.getRadiusY();
				ry *= zoomFactor;
				layer->viewport.setRadiusY(ry);
			}
		}
	};
	ViewportController viewportController(window, layer2d, mainNode);
	
	
	
	
	// Main Loop
	while(window->isActive()){
		
		float tpf = window->tick(60);
		viewportController.update(tpf);
		window->update(tpf);
		
	}

	
	delete window;
	
	printf("Process Exiting.\n");
	
	return 0;
}
