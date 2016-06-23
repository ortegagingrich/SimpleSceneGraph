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

int main(){
	printf("\nEntry Point for jvisu demos.\n");
	
	JWindow *window = new JWindow(640, 480);
	
	
	/*
	 * Try to make a quit callback function
	 */
	class QuitCallback : JEventCallback {
	public:
		QuitCallback(JWindow *window): JEventCallback(window) {};
		virtual ~QuitCallback(){};
		virtual void callback(SDL_Event event){
			if(event.type != SDL_QUIT) return;
			printf("Disposing for real.\n");
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
