/* 
 * ButtonManager class definition
 * 
 * The implementation is in button.cpp along with the other button code.  A separate
 * header is required to avoid circular dependency with layer.h
 */
#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

class InputEvent;
class ComponentButton2D;


class SHARED_EXPORT ButtonManager {
public:
	
	ButtonManager();
	
	void considerButton(InputEvent *event, ComponentButton2D *button, float priority);
	void processEvent(InputEvent *event, float tpf);
	
private:
	ComponentButton2D *topButton;
	float topPriority;
};

#endif
