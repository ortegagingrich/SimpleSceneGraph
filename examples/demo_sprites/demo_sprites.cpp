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


class RectDrawerClickCallback;


class RectDrawer {
friend class RectDrawerClickCallback;
public:
	JWindow *window;
	Layer2D *layer, *hud;
	Node2D *rootNode, *mainNode;
	ComponentSpriteSimple2D *fpsCounter;
	
	RectDrawer(JWindow *win, Layer2D *lay);
	
	void update(float tpf);
private:
	
	
	// Callbacks
	RectDrawerClickCallback *clickCallback;
};



class RectDrawerClickCallback : public MouseButtonCallback, public TextureOwner {
public:
	
	Texture *landTexture;
	
	RectDrawerClickCallback(RectDrawer *dr, Layer2D *layer):
		MouseButtonCallback(layer),
		drawer(dr)
	{
		landTexture = Texture::createFromFile("assets/test/land.png", drawer->window);
		//landTexture = Texture::createSolidColor(20, 20, drawer->window, 0xff, 0xff, 0, 0);
		
		if(landTexture != NULL) landTexture->addOwner(this);
	};
	
	virtual ~RectDrawerClickCallback(){
		if(landTexture != NULL){
			landTexture->removeOwner(this);
		}
	};
	
	virtual void removeTexture(Texture *tex){
		if(tex == landTexture){
			landTexture = NULL;
		}
	}
	
	virtual void callback(MouseButtonEvent *event, float tpf){
		bool fixed, useLand;
		if(event->isLeftButton()){
			fixed = false;
		}else{
			fixed = true;
		}
		
		
		useLand = !drawer->window->isKeyPressed(SDLK_SPACE);
		if(landTexture == NULL) useLand = false;
		
		if(event->isReleased()){
			// Add new Rectangle Here
			printf("Adding new sprite at world coordinates: ");
			Vector2f wc = event->getWorldCoordinates(drawer->layer);
			printf("(%f, %f)\n", wc.x, wc.y);
			
			Uint8 r, g, b, a;
			r = (int) 200 * wc.x;
			b = (int) 200 * wc.y;
			g = 0x00;
			a = 0xff;
			Texture *texture;
			if(useLand){
				//texture = Texture::createFromFile("assets/test/land.png", drawer->window);
				texture = landTexture;
				//texture->load();
			}else{
				texture = Texture::createSolidColor(40, 25,drawer->window, r, g, b, a); 
			}
			
			ComponentSpriteSimple2D *newrect = new ComponentSpriteSimple2D(texture);
			newrect->position = drawer->mainNode->computeRelativePosition(wc);
			newrect->width = 0.1;
			newrect->height = 0.1;
			//newrect->height = 0.0785;
			newrect->centerOffset.set(0.05, 0.05);
			//newrect->centerOffset.set(0.05, 0.03725);
			newrect->fixedSize = fixed;
			drawer->mainNode->attachChild(newrect);
		}
	};
private:
	RectDrawer *drawer;
};



RectDrawer::RectDrawer(JWindow *win, Layer2D *lay): window(win), layer(lay) {
	rootNode = layer->getRootNode();
	
	mainNode = new Node2D();
	rootNode->attachChild(mainNode);
	//mainNode->hide();
	
	// Reference Line
	ComponentLine2D *refLine = new ComponentLine2D();
	refLine->colorBlue = 0x00;
	refLine->colorGreen = 0x00;
	refLine->startCoordinates = Vector2f(0, 0);
	refLine->endCoordinates = Vector2f(0, 100);
	rootNode->attachChild(refLine);
	
	// Main Node Rotation Line
	ComponentLine2D *rotLine = new ComponentLine2D();
	rotLine->colorGreen = 0x00;
	rotLine->colorRed = 0x88;
	rotLine->zLevel = -50;
	rotLine->startCoordinates = Vector2f(0, 0);
	rotLine->endCoordinates = Vector2f(0, 100);
	mainNode->attachChild(rotLine);
	
	
	// Transparent Sprite
	Texture *fairyTexture = Texture::createFromFile("assets/test/Red.png", window);
	
	ComponentSpriteSimple2D *fairy = new ComponentSpriteSimple2D(fairyTexture);
	rootNode->attachChild(fairy);
	fairy->width = 0.05;
	fairy->height = 0.05;
	fairy->zLevel = 100;
	
	
	/*
	 * FPS Counter
	 */
	hud = new Layer2D("hud");
	window->addLayerTop(hud);
	
	fpsCounter = new ComponentSpriteSimple2D();
	hud->getRootNode()->attachChild(fpsCounter);
	fpsCounter->position = Vector2f(-window->getAspectRatio(), 1.0f);
	
	
	// Callbacks
	clickCallback = new RectDrawerClickCallback(this, layer);
}


void RectDrawer::update(float tpf){
	// Temporary Texture:
	//Texture *text_texture = Texture::createSolidColor(128, 12, window, 0xff, 0, 0, 0xff);
	std::stringstream stringstream;
	stringstream << "FPS: " << window->getFPS();
	
	std::string text = stringstream.str();
	
	Texture *text_texture = Texture::createFromText(
		text,
		"assets/font/LiberationSerif-Regular.ttf",
		24,
		window,
		0xff,
		0xff,
		0xff,
		0xff
	);
	
	float ratio = (float) text_texture->width / (float) text_texture->height;
	fpsCounter->setTexture(text_texture);
	fpsCounter->height = 0.1f;
	fpsCounter->width = fpsCounter->height * ratio;
}


/*
 * This function is the primary entry point for demo/testing purposes.
 */
int main(int argc, char* argv[]){
	printf("\nEntry Point for jvisu demos.\n");
	
	JWindow *window = new JWindow(1280, 720, USE_HARDWARE_ACCELERATION);
	
	
	/*
	 * Background Handling
	 */
	//LayerBackground *background = (LayerBackground*) window->getLayerById("background");
	//background->setBackgroundColor(0x55, 0x77, 0xbb, 0x00);
	
	
	/*
	 * Layer2D Test
	 */
	Layer2D *layer2d = new Layer2D("2dlayer");
	window->addLayerTop(layer2d);
	
	
	RectDrawer drawer(window, layer2d);
	
	
	
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
	ViewportController viewportController(window, layer2d, drawer.mainNode);
	
	
	
	
	// Main Loop
	while(window->isActive()){
		
		float tpf = window->tick(60);
		
		drawer.update(tpf);
		viewportController.update(tpf);
		window->update(tpf);
	}

	
	delete window;
	
	printf("Process Exiting.\n");
	
	return 0;
}
