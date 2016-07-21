#include "utils.h"
#include "geometry.h"
#include "vectormath.h"

/*
 * The Cohen-Sutherland algorithm for line clipping
 */

// Offset Codes
const static int LEFT = 1;
const static int RIGHT = 2;
const static int DOWN = 4;
const static int UP = 8;

// Helper function
static int computeOutcode(float x, float y, const Rect2f &rect){
	
	int code = 0;
	
	if(x < rect.xMin){
		code |= LEFT;
	}else if(x > rect.xMax){
		code |= RIGHT;
	}
	if(y < rect.yMin){
		code |= DOWN;
	}else if(y > rect.yMax){
		code |= UP;
	}
	
	return code;
}


bool cohen_sutherland(Line2f &line, Rect2f rect){
	Vector2f p1, p2;
	p1 = line.startPoint;
	p2 = line.endPoint;
	
	int outcode1 = computeOutcode(p1.x, p1.y, rect);
	int outcode2 = computeOutcode(p2.x, p2.y, rect);
	
	// Iterate until both points are inside the rectangle (outcodes are zero)
	while(true){
		if(!(outcode1 | outcode2)){
			// both points are inside
			break;
		}else if(outcode1 & outcode2){
			// definitely no intersection
			return false;
		}
		
		// Decide which point to fix
		int fixcode = outcode1 ? outcode1 : outcode2;
		
		float x, y;
		
		// Determine point of intersection
		if (fixcode & UP) {
			x = p1.x + (p2.x - p1.x) * (rect.yMax - p1.y) / (p2.y - p1.y);
			y = rect.yMax;
		} else if (fixcode & DOWN) {
			x = p1.x + (p2.x - p1.x) * (rect.yMin - p1.y) / (p2.y - p1.y);
			y = rect.yMin;
		} else if (fixcode & RIGHT) {
			y = p1.y + (p2.y - p1.y) * (rect.xMax - p1.x) / (p2.x - p1.x);
			x = rect.xMax;
		} else if (fixcode & LEFT) {
			y = p1.y + (p2.y - p1.y) * (rect.xMin - p1.x) / (p2.x - p1.x);
			x = rect.xMin;
		}
		
		if (fixcode == outcode1) {
			p1.x = x;
			p1.y = y;
			outcode1 = computeOutcode(p1.x, p1.y, rect);
		} else {
			p2.x = x;
			p2.y = y;
			outcode2 = computeOutcode(p2.x, p2.y, rect);
		}
	}
	
	line.startPoint = p1;
	line.endPoint = p2;
	return true;
	
}


