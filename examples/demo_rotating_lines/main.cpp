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


class LineDrawerDragCallback;
class LineDrawerClickCallback;


class LineDrawer {
friend class LineDrawerDragCallback;
friend class LineDrawerClickCallback;
public:
	Window *window;
	Layer2D *layer;
	Node2D *rootNode, *mainNode, *previewNode;
	
	LineDrawer(Window *win, Layer2D *lay);
private:
	
	ComponentPoint2D *downPoint;
	ComponentLine2D *previewLine;
	
	// Callbacks
	LineDrawerDragCallback *dragCallback;
	LineDrawerClickCallback *clickCallback;
};



class LineDrawerDragCallback : public MouseMotionCallback {
public:
	LineDrawerDragCallback(LineDrawer *dr, Layer2D *layer):
		MouseMotionCallback(layer),
		drawer(dr)
	{}
	virtual void callback(MouseMotionEvent *event, float tpf){
		if(event->leftButtonPressed()){
			Vector2f wc = event->getWorldCoordinates(drawer->layer);
			drawer->previewLine->endCoordinates = wc;
		}
	}
private:
	LineDrawer *drawer;
};


class LineDrawerClickCallback : public MouseButtonCallback {
public:
	LineDrawerClickCallback(LineDrawer *dr, Layer2D *layer):
		MouseButtonCallback(layer),
		drawer(dr)
	{}
	virtual void callback(MouseButtonEvent *event, float tpf){
		if(event->isLeftButton()){
			if(event->isPressed()){
				Vector2f wc = event->getWorldCoordinates(drawer->layer);
				drawer->downPoint->position = wc;
				drawer->previewLine->startCoordinates = wc;
				drawer->previewLine->endCoordinates = wc;
				
				
				Vector2f dp = drawer->downPoint->position;
				
				
				drawer->previewNode->show();
			}else{
				
				// Draw results here
				ComponentPoint2D *sp, *ep;
				sp = new ComponentPoint2D();
				ep = new ComponentPoint2D();
				sp->colorBlue = 0x00;
				sp->colorGreen = 0x00;
				ep->colorBlue = 0x00;
				ep->colorGreen = 0x00;
				
				sp->position = drawer->downPoint->position;
				sp->position = drawer->mainNode->computeRelativePosition(sp->position);
				ep->position = event->getWorldCoordinates(drawer->layer);
				ep->position = drawer->mainNode->computeRelativePosition(ep->position);
				sp->zLevel = 1;
				ep->zLevel = 1;
				
				drawer->mainNode->attachChild(sp);
				drawer->mainNode->attachChild(ep);
				
				ComponentLine2D *l;
				l = new ComponentLine2D();
				l->startCoordinates = sp->position;
				l->endCoordinates = ep->position;
				l->colorRed = 0x00;
				l->colorGreen = 0x00;
				drawer->mainNode->attachChild(l);
				
				
				drawer->previewNode->hide();
			}
		}
	}
private:
	LineDrawer *drawer;
};



LineDrawer::LineDrawer(Window *win, Layer2D *lay): window(win), layer(lay) {
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
	
	
	/*
	 * Preview Components
	 */
	
	previewNode = new Node2D();
	previewNode->zLevel = 2;
	previewNode->hide();
	
	downPoint = new ComponentPoint2D();
	downPoint->colorRed = 0xff;
	downPoint->colorBlue = 0xff;
	downPoint->colorGreen = 0xff;
	previewNode->attachChild(downPoint);
	
	previewLine = new ComponentLine2D();
	previewLine->colorRed = 0xff;
	previewLine->colorBlue = 0xff;
	previewLine->colorGreen = 0xff;
	previewNode->attachChild(previewLine);
	
	rootNode->attachChild(previewNode);
	
	
	
	// Callbacks
	
	dragCallback = new LineDrawerDragCallback(this, layer);
	clickCallback = new LineDrawerClickCallback(this, layer);
}



/*
 * This function is the primary entry point for demo/testing purposes.
 */
int main(int argc, char* argv[]){
	printf("\nEntry Point for jvisu demos.\n");
	
	Window *window = new Window(1280, 720, USE_HARDWARE_ACCELERATION);
	
	
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
	
	
	LineDrawer drawer(window, layer2d);
	
	
	
	/*
	 * Callback for moving the viewport
	 */
	class ViewportController {
	public:
		const float panSpeed = 0.025;
		const float rotSpeed = 0.05;
		const float zoomFactor = 1.05;
		Window *window;
		Layer2D *layer, *hud;
		Node2D *mainNode;
		ComponentSpriteText2D *fpsCounter;
		
		ViewportController(Window *win, Layer2D *l, Node2D *m):
			window(win),
			layer(l),
			mainNode(m)
		{
			hud = new Layer2D("hud");
			window->addLayerTop(hud);
			
			fpsCounter = new ComponentSpriteText2D(window);
			hud->getRootNode()->attachChild(fpsCounter);
			fpsCounter->position = Vector2f(-window->getAspectRatio(), 1.0f);
		}
		
		
		void update(float tpf){
			
			/*
			 * FPS Counter
			 */
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
			
			
			Vector2f newCenter = layer->viewport.getCenter();
			float vdiff = panSpeed * layer->viewport.getRadiusY();
			float hdiff = panSpeed * layer->viewport.getRadiusX();
			
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
			
			
			
			// Zooming (not scaling)
			
			if(window->isKeyPressed(SDLK_p)){
				float ry = layer->viewport.getRadiusY();
				ry /= zoomFactor;
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
	
	
	
	/*
	 * Debug Coordinate-obtaining callback
	 */
	
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
