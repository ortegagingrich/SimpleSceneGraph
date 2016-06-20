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
	
#ifdef __linux__
	usleep(3*1000*1000);
#endif
#ifdef _WIN32
	Sleep(3*1000);
#endif
	
	delete window;
	
	printf("Process Exiting.\n");
	
	return 0;
}
