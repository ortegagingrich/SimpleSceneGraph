/**
 * Some miscellaneous utils, including some implementations of useful algorithms
 */
#ifndef UTILS_H
#define UTILS_H

#include "shared_exports.h"

#include "geometry.h"
#include "vectormath.h"


namespace ssg{
	bool cohen_sutherland(Line2f &line, Rect2f rect);
}


#endif
