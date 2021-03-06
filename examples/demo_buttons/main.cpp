#include <cstdio>
#include <cmath>
#include <sstream>
#include <string>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "ssg.h"


using namespace ssg;



#ifdef __linux__
const bool USE_HARDWARE_ACCELERATION = true;
#endif
#ifdef _WIN32
const bool USE_HARDWARE_ACCELERATION = true;
#endif


std::string TEST_FONT = "assets/font/LiberationSerif-Regular.ttf";


/*
 * Class Definitions for this Demo
 */

class FPSCounter : public ComponentSpriteText2D {
public:
	FPSCounter(Window *win): ComponentSpriteText2D(win){
		fontPath = TEST_FONT;
		fontSize = 24;
		text = "[FPS]";
		
		position = Vector2f(-win->getAspectRatio(), 1.0f);
		height = 0.1f;
	}
	
	virtual void onUpdate(Layer2D *layer, float tpf){
		
		// Make new FPS string
		std::stringstream stream;
		stream << "FPS: " << window->getFPS();
		text = stream.str();
	}
};



class TestButton : public ComponentDraggable2D {
public:
	std::string label;
	Window *window;
	
	TestButton(Window *win, std::string txt):
		ComponentDraggable2D(win),
		label(txt),
		window(win)
	{
	
		Texture *base, *overlay, *pressed;
		base = Texture::createFromFile("assets/test/button.png", win);
		overlay = Texture::createSolidColor(256, 256, win, 0xbb, 0x88, 0xbb, 0x55);
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
	printf("\nEntry Point for ssg demos.\n");
	
	//Window *window = new Window(1280, 720, USE_HARDWARE_ACCELERATION);
	Window *window = new Window(1280, 720, USE_HARDWARE_ACCELERATION);
	
	
	
	
	/*
	 * Layer2D Test
	 */
	Layer2D *layer2d = new Layer2D("2dlayer");
	window->addLayerTop(layer2d);
	
	Node2D *mainNode = new Node2D();
	layer2d->getRootNode()->attachChild(mainNode);
	
	
	/*
	 * Fixed Layer
	 */
	Layer2D *hud = new Layer2D("hud");
	window->addLayerTop(hud);
	hud->getRootNode()->attachChild(new FPSCounter(window));
	
	TestButton *button1 = new TestButton(window, "Fixed Button");
	button1->setFont(TEST_FONT);
	button1->setText("Fixed");
	button1->setFontSize(48);
	button1->height = 0.2f;
	button1->width = -1;
	hud->getRootNode()->attachChild(button1);
	
	
	TestButton *button2 = new TestButton(window, "pixel");
	button2->setFont(TEST_FONT);
	button2->setText("p");
	button2->setFontSize(128);
	button2->width = -1;
	button2->height = -1;
	button2->zLevel = -2;
	mainNode->attachChild(button2);
	
	
	TestButton *button3 = new TestButton(window, "nh");
	button3->setFont(TEST_FONT);
	button3->setText("nh");
	button3->setFontSize(128);
	button3->width = -1;
	button3->height = 0.05f;
	button3->position.x = 0.2f;
	button3->zLevel = 2.0f;
	mainNode->attachChild(button3);
	
	
	TestButton *button4 = new TestButton(window, "nw");
	button4->setFont(TEST_FONT);
	button4->setText("nw");
	button4->setFontSize(128);
	button4->width = 0.05f;
	button4->height = -1;
	button4->position.x = -0.2f;
	button4->zLevel = 4.0f;
	mainNode->attachChild(button4);
	
	
	TestButton *button5 = new TestButton(window, "nb");
	button5->setFont(TEST_FONT);
	button5->setText("nb");
	button5->setFontSize(128);
	button5->width = 0.1f;
	button5->height = 0.1f;
	button5->zLevel = 5;
	mainNode->attachChild(button5);
	
	
	TestButton *button6 = new TestButton(window, "fh");
	button6->setFont(TEST_FONT);
	button6->setText("fh");
	button6->setFontSize(128);
	button6->width = -1;
	button6->height = 0.4f;
	button6->fixedSize = true;
	button6->zLevel = 6.0f;
	mainNode->attachChild(button6);
	
	
	TestButton *button7 = new TestButton(window, "fw");
	button7->setFont(TEST_FONT);
	button7->setText("fw");
	button7->setFontSize(128);
	button7->width = 0.1f;
	button7->height = -1;
	button7->fixedSize = true;
	button7->zLevel = 7;
	mainNode->attachChild(button7);
	
	
	TestButton *button8 = new TestButton(window, "fb");
	button8->setFont(TEST_FONT);
	button8->setText("fb");
	button8->setFontSize(128);
	button8->width = 0.2f;
	button8->height = 0.2f;
	button8->fixedSize = true;
	button8->zLevel = 8;
	mainNode->attachChild(button8);
	
	
	/*
	 * Callback for moving the viewport
	 */
	class ViewportController {
	public:
		const float panSpeed = 0.025;
		const float rotSpeed = 0.05;
		const float zoomFactor = 1.05;
		Window *window;
		Layer2D *layer;
		Node2D *mainNode;
		ViewportController(Window *win, Layer2D *l, Node2D *m):
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
			
			
			// Scaling
			if(window->isKeyPressed(SDLK_i)){
				mainNode->scale.scale(1, zoomFactor);
			}
			if(window->isKeyPressed(SDLK_j)){
				mainNode->scale.scale(1.0f/zoomFactor, 1);
			}
			if(window->isKeyPressed(SDLK_k)){
				mainNode->scale.scale(1, 1.0f/zoomFactor);
			}
			if(window->isKeyPressed(SDLK_l)){
				mainNode->scale.scale(zoomFactor, 1);
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
