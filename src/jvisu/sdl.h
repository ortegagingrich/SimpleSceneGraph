/**
 * Contains a wrapper include for SDL depending upon the platform
 */
#ifndef SDL_H
#define SDL_H

#ifdef __linux__
#include <SDL2/SDL.h>
#endif
#ifdef _WIN32
#include <SDL.h>
#endif

#endif