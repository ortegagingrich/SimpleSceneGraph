#include <cstdio>
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>

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


const std::string TEST_FONT = "assets/font/LiberationSerif-Regular.ttf";


static std::string load_lorem_ipsum(){
	std::ifstream filestream("assets/text/lorem_ipsum.txt");
	std::stringstream buffer;
	buffer << filestream.rdbuf();
	return buffer.str();
}



/*
 * Class Definitions for this Demo
 */

class FPSCounter : public ComponentSpriteText2D {
public:
	FPSCounter(JWindow *win): ComponentSpriteText2D(win){
		fontPath = TEST_FONT;
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


/*
 * A button to toggle the visibility of a component.
 */

class ToggleButton : public ComponentButtonSimple2D {
public:
	std::string label;
	JWindow *window;
	Component2D *target; // The compoment whose visibility is to be toggled.
	
	
	ToggleButton(JWindow *win, std::string txt):
		ComponentButtonSimple2D(win),
		label(txt),
		window(win),
		target(NULL)
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
	 * Callback
	 */
	
	virtual void onLeftClick(MouseButtonEvent *event, float tpf){
		if(target != NULL) target->toggleVisibility();
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
	
	
	ComponentTextBox2D *loremIpsum = new ComponentTextBox2D(window);
	loremIpsum->position.set(-0.7f, 0.5f);
	loremIpsum->width = 1.4f;
	loremIpsum->height = 1.0f;
	loremIpsum->lineCount = 4;
	loremIpsum->spacingRatio = 1.0f;
	loremIpsum->fontPath = TEST_FONT;
	loremIpsum->fontSize = 48;
	loremIpsum->text = load_lorem_ipsum();
	loremIpsum->hide();
	mainNode->attachChild(loremIpsum);
	
	
	// Frame
	Node2D *frame = new Node2D();
	mainNode->attachChild(frame);
	frame->attachChild(new ComponentLine2D(-0.7f,  0.5f,  0.7f,  0.5f));
	frame->attachChild(new ComponentLine2D(-0.7f,  0.5f, -0.7f, -0.5f));
	frame->attachChild(new ComponentLine2D( 0.7f, -0.5f, -0.7f, -0.5f));
	frame->attachChild(new ComponentLine2D( 0.7f, -0.5f,  0.7f,  0.5f));
	
	
	
	/*
	 * Fixed Layer
	 */
	Layer2D *hud = new Layer2D("hud");
	window->addLayerTop(hud);
	hud->rootNode->attachChild(new FPSCounter(window));
	
	ToggleButton *button1 = new ToggleButton(window, "Fixed Button");
	button1->setFont(TEST_FONT);
	button1->setText("Toggle Lorem Ipsum");
	button1->setFontSize(48);
	button1->position.set(-0.4f, 1.0f);
	button1->height = 0.1f;
	button1->width = 0.8f;
	button1->target = loremIpsum;
	hud->rootNode->attachChild(button1);
	
	
	
	
	
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
