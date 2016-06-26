/**
 * Contains a wrapper include for SDL depending upon the platform
 */
#ifndef SDL_H
#define SDL_H

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#endif


int create_SDL_window(SDL_Window **win, const char *window_name, int sx, int sy);
void remove_SDL_window(SDL_Window *window);

SDL_Renderer *create_SDL_renderer(SDL_Window *window);

void print_SDL_error();

#endif
