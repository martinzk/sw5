#ifndef MenuManager_H
#define MenuManager_H

#include <Menu.h>
#include <ADButtons.h>

class MenuManager
{
public:
    static MenuManager* GetMenuManager();
    ~MenuManager()
    {
        instanceFlag = false;
    }
    void AddRootMenu(Menu* menu);
    void SelectPush();
private:
    MenuManager(); 
    static MenuManager* menuManager;
    static bool instanceFlag;
    void Select(int id);
    void up();
    void down();
    void execute();
    void back();
    void home();
    Menu* rootMenu;
    Menu* currentMenu;
    ADButtons keypad;
};

#endif
