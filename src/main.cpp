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
 * Class Definitions for this Demo
 */


class LineDrawerDragCallback;
class LineDrawerClickCallback;


class LineDrawer {
friend class LineDrawerDragCallback;
friend class LineDrawerClickCallback;
public:
	LineDrawer(JWindow *win, Layer2D *lay);
private:
	JWindow *window;
	Layer2D *layer;
	Node2D *rootNode, *mainNode, *previewNode;
	
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
	virtual void callback(MouseMotionEvent *event){
		if(event->leftButtonPressed()){
			printf("Dragging... [%d, %d]\n", event->screenX, event->screenY);
			
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
	virtual void callback(MouseButtonEvent *event){
		if(event->isLeftButton()){
			if(event->isPressed()){
				Vector2f wc = event->getWorldCoordinates(drawer->layer);
				drawer->downPoint->position = wc;
				drawer->previewLine->startCoordinates = wc;
				
				
				Vector2f dp = drawer->downPoint->position;
				printf("(%f, %f)\n", dp.x, dp.y);
				
				
				drawer->previewNode->show();
			}else{
				printf("UP\n");
				
				// Draw results here
				ComponentPoint2D *sp, *ep;
				sp = new ComponentPoint2D();
				ep = new ComponentPoint2D();
				sp->colorRed = 0xff;
				ep->colorRed = 0xff;
				
				sp->position = drawer->downPoint->position;
				ep->position = event->getWorldCoordinates(drawer->layer);
				
				drawer->mainNode->attachChild(sp);
				drawer->mainNode->attachChild(ep);
				
				ComponentLine2D *l;
				l = new ComponentLine2D();
				l->startCoordinates = sp->position;
				l->endCoordinates = ep->position;
				l->colorBlue = 0xff;
				drawer->mainNode->attachChild(l);
				
				
				drawer->previewNode->hide();
			}
		}
	}
private:
	LineDrawer *drawer;
};



LineDrawer::LineDrawer(JWindow *win, Layer2D *lay): window(win), layer(lay) {
	rootNode = layer->rootNode;
	
	mainNode = new Node2D();
	rootNode->attachChild(mainNode);
	//mainNode->hide();
	
	/*
	 * Preview Components
	 */
	
	previewNode = new Node2D();
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
	
	
	LineDrawer drawer(window, layer2d);
	
	
	
	
	
	
	class OnRightClick : public MouseButtonCallback {
	public:
		Layer2D *layer;
		OnRightClick(Layer2D *l): MouseButtonCallback(l), layer(l) {}
		
		virtual void callback(MouseButtonEvent *event){
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
