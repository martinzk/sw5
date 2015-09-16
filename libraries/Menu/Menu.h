#ifndef ____Menu__
#define ____Menu__
#include <Arduino.h>
#include <MenuItem.h>
#include <display.h>
#include <Configurations.h>
#include <setupConstants.h>


class Menu : public IMenuItem {
public:
    Menu(char* name);
    Menu();
    void Select();
    void Add(IMenuItem* menuItem);
    void SetParent(IMenuItem* parent);
    char* GetName();
    IMenuItem* GetParent();
    IMenuItem* GetMenuChild();
    int GetMenuSize();
    char* GetType();
    int index, menuRowCounter;
private:
	void append(char* first, char character, int len);
    int menuSize;
    void Show();
    char *name;
    IMenuItem* parent;
    IMenuItem* children[MAX_NUMBER_OF_MenuItem_ITEMS];
    Display* display;
};

#endif