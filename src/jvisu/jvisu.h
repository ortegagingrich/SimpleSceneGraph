/**
 * This is the main include file for jvisu.
 */

#ifndef JVISU_H
#define JVISU_H


namespace jvisu {};


#define internal private
#define INCLUDE



#include "window.h"
#include "layer.h"
#include "texture.h"
#include "scene_graph.h"
#include "text.h"
#include "button.h"
#include "callback.h"
#include "input.h"
#include "vectormath.h"
#include "geometry.h"


// Hopefully this can be engineered out later
#include "sdl.h"



#undef internal
#undef INCLUDE


#endif
