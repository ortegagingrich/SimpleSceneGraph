/* 
 * ButtonManager class definition
 * 
 * The implementation is in button.cpp along with the other button code.  A separate
 * header is required to avoid circular dependency with layer.h
 */



class SHARED_EXPORT ButtonManager {
public:
	
	ButtonManager();
	
	void considerButton(InputEvent *event, ComponentButton2D *button, float priority);
	void processEvent(InputEvent *event, float tpf);
	
private:
	ComponentButton2D *topButton;
	float topPriority;
};
