/**
 * This is the main include file for jvisu.
 */

#ifndef JVISU_H
#define JVISU_H


namespace jvisu {};


#define internal public


#include "vectormath.h"
#include "geometry.h"

#include "window.h"
#include "layer.h"
#include "viewport.h"
#include "texture.h"

#include "scene_graph.h"
#include "text.h"
#include "button.h"

#include "callback.h"
#include "input.h"


// Hopefully this can be engineered out later
#include "sdl.h"



#undef internal


#endif
