#ifndef ____Action__
#define ____Action__
#include <Arduino.h>
#include <MenuItem.h>

class Action : public IMenuItem {
public:
	Action(char* name, void (*action)());
	void Select();
	char* GetName();
	void SetParent(IMenuItem* parent);
	char* GetType();
private:
	char* name;
	void (*action)();
	IMenuItem* parent;
};

#endif