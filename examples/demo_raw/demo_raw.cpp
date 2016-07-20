/**
 * Some tests/prototyping using raw SDL (not jvisu)
 */
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdio>
#include "jvisu.h"


#ifdef __linux__
const bool HARDWARE_ACCELERATION = false;
#endif
#ifdef _WIN32
const bool HARDWARE_ACCELERATION = true;
#endif



volatile bool ACTIVE = true;

SDL_Window *WINDOW = NULL;
SDL_Renderer *RENDERER = NULL;
SDL_Texture *TEXTURE = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

float ANGLE = 0;
float DELTA_ANGLE = 90;

float OFFSET_X = 0;
float OFFSET_Y = 0;
float OFFSET_DELTA = 300;

float FACTOR_X = 1;
float FACTOR_Y = 1;
float FACTOR_FACTOR = 1.01f;

int TARGET_X = 0;
int TARGET_Y = 0;
int TARGET_W = 256;
int TARGET_H = 256;



static void clear_screen(){
	SDL_SetRenderDrawColor(RENDERER, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(RENDERER, NULL);
}


static void setup(){
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	
	// Create Window
	WINDOW = SDL_CreateWindow("Demo Raw", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                           SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	// Create Renderer
	Uint32 rflags = HARDWARE_ACCELERATION ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE;
	RENDERER = SDL_CreateRenderer(WINDOW, -1, rflags);
	if(SDL_SetRenderDrawBlendMode(RENDERER, SDL_BLENDMODE_BLEND) < 0){
		print_SDL_error();
	}
	
	
	//Create Texture
	SDL_Surface *loaded = IMG_Load("assets/test/land.png");
	TEXTURE = SDL_CreateTextureFromSurface(RENDERER, loaded);
	SDL_SetTextureBlendMode(TEXTURE, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(loaded);
	
	
	// Fill Window with black
	clear_screen();
}


static void cleanup(){
	
	if(TEXTURE != NULL) SDL_DestroyTexture(TEXTURE);
	if(RENDERER != NULL) SDL_DestroyRenderer(RENDERER);
	if(WINDOW != NULL) SDL_DestroyWindow(WINDOW);
	SDL_Quit();
}


static bool key_pressed(SDL_Keycode keycode){
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
	return keystate[scancode] != 0;
}


static void refresh(float tpf){
	clear_screen();
	
	if(!key_pressed(SDLK_SPACE)){
		SDL_Rect rect;
		rect.x = (int) TARGET_X + OFFSET_X;
		rect.y = (int) TARGET_Y + OFFSET_Y;
		rect.w = (int) TARGET_W * FACTOR_X;
		rect.h = (int) TARGET_H * FACTOR_Y;
		
		for(int i = 0; i < 1; i++){
			render_copy_clip(RENDERER, TEXTURE, NULL, &rect, -ANGLE);
		}
	}
	
	SDL_RenderPresent(RENDERER);
}





static void update_input(float tpf){
	SDL_PumpEvents();
	
	if(key_pressed(SDLK_w)){
		OFFSET_Y += OFFSET_DELTA * tpf;
	}
	if(key_pressed(SDLK_s)){
		OFFSET_Y -= OFFSET_DELTA * tpf;
	}
	if(key_pressed(SDLK_a)){
		OFFSET_X += OFFSET_DELTA * tpf;
	}
	if(key_pressed(SDLK_d)){
		OFFSET_X -= OFFSET_DELTA * tpf;
	}
	if(key_pressed(SDLK_p)){
		FACTOR_X *= FACTOR_FACTOR;
		FACTOR_Y *= FACTOR_FACTOR;
	}
	if(key_pressed(SDLK_m)){
		FACTOR_X /= FACTOR_FACTOR;
		FACTOR_Y /= FACTOR_FACTOR;
	}
	if(key_pressed(SDLK_q)){
		ANGLE += DELTA_ANGLE * tpf;
	}
	if(key_pressed(SDLK_e)){
		ANGLE -= DELTA_ANGLE * tpf;
	}
	
	if(key_pressed(SDLK_ESCAPE)){
		ACTIVE = false;
	}
}


long FRAMECOUNT = 0;
float FPS[60];
const int TARGET_FPS = 60;
const float TARGET_TPF = 1.0 / TARGET_FPS;
long LAST_TIME;



long time_ms();

#ifdef _WIN32

#include <windows.h>

long time_ms(){
	return timeGetTime();
}

#else

#include <sys/time.h>

long time_ms(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif


static float tick(){
	
	FRAMECOUNT++;
	
	
	float tpf = 1.0 / 60.0; // DEFAULT VALUE
	
	long current_time = time_ms();
	if(FRAMECOUNT > 0){
		tpf = (current_time - LAST_TIME) / 1000.0;
		LAST_TIME = current_time;
	}
	
	
	float diff = TARGET_TPF - tpf;
	if(diff > 0){
		tpf = TARGET_TPF;
#ifdef __linux__
		usleep((int) (diff * 1000 * 1000));
#endif
#ifdef _WIN32
		Sleep((int) (diff * 1000));
#endif		
	}
	

	
	
	float framerate = 1.0 / tpf; 
	
	FPS[FRAMECOUNT % 60] = framerate;
	
	
	if(FRAMECOUNT % 600 == 0){
		float average_fps = 0;
		
		for(int i = 0; i < 60; i++){
			average_fps += FPS[i];
		}
		
		average_fps /= 60;
		float area = TARGET_W * TARGET_H * FACTOR_X * FACTOR_Y;
		float regression = 1000000 * tpf / area;
		
		printf("FPS: %f, Area: %f, Time per Unit Area: %f\n", average_fps, area, regression);
	}
	
	
	return tpf;
}




int main(){
	
	printf("\n\nRaw SDL Demo\n");
	setup();
	
	
	// Main Loop
	while(ACTIVE){
		float tpf = tick();
		refresh(tpf);
		update_input(tpf);
	}
	
	
	cleanup();
	printf("\n\n");
	return 0;
}

