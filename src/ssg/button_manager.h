/* 
 * ButtonManager class definition
 * 
 * The implementation is in button.cpp along with the other button code.  A separate
 * header is required to avoid circular dependency with layer.h
 */

#include "shared_exports.h"

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

namespace ssg {
	
	class InputEvent;
	class ComponentButton2D;


	class SHARED_EXPORT ButtonManager {
	public:
	
		ButtonManager();
	
		void considerButton(ComponentButton2D *button, float priority, InputEvent *event);
		void processEvent(InputEvent *event, float tpf);
	
	private:
		ComponentButton2D *topButton;
		float topPriority;
	};
}

#endif
