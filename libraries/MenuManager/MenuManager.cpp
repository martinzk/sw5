#include <MenuManager.h>

bool MenuManager::instanceFlag = false;
MenuManager* MenuManager::menuManager = NULL;

MenuManager* MenuManager::GetMenuManager()
{
    if(!instanceFlag)
    {
        menuManager = new MenuManager();
        instanceFlag = true;
        return menuManager;
    }
    return menuManager;
}

MenuManager::MenuManager()
{
    keypad.SetInPin(AD_KEYPAD_PIN);
	keypad.AddButton(1, 0, 40);
	keypad.AddButton(2, 141, 40);
	keypad.AddButton(3, 328, 40);
	keypad.AddButton(4, 503, 40);
  	keypad.AddButton(5, 740, 40);
}

void MenuManager::AddRootMenu(Menu* rootMenu)
{
	this->currentMenu = rootMenu;
	this->rootMenu = rootMenu;
	this->currentMenu->Select();
}

void MenuManager::SelectPush()
{
	int buttonPress = 0;

	buttonPress = keypad.RegisterPush();
	switch (buttonPress)
	{
		case 1:
 			this->back();
 			break;
		case 2:
			this->up();
			break;
 		case 3:
 			this->down();
 			break;
 		case 4:
 			this->execute();
 			break;
 		case 5:
 			this->home();		
			default:
 			break;
	}
}

void MenuManager::up()
{
	int row = this->currentMenu->index - this->currentMenu->menuRowCounter;
	if(this->currentMenu->index != 0)
	{
		if(row == 0)
		{
			this->currentMenu->menuRowCounter--;
		}

		this->currentMenu->index--;
		this->currentMenu->Select();
	}
}

void MenuManager::down() 
{
	int row = this->currentMenu->index - this->currentMenu->menuRowCounter;
	if(this->currentMenu->index +1 < this->currentMenu->GetMenuSize())
	{
		if(row == 3)
		{
			this->currentMenu->menuRowCounter++;
		}

		this->currentMenu->index++;
		this->currentMenu->Select();
	}
}

void MenuManager::execute()
{
	if(this->currentMenu->GetMenuSize() >0)
	{
		IMenuItem* child = this->currentMenu->GetMenuChild();

		if(child->GetType() == currentMenu->GetType())
		{
			this->currentMenu = static_cast<Menu*>(child);
		}
		child->Select();
	}
}

void MenuManager::back()
{
	this->currentMenu->index = 0;
	this->currentMenu->menuRowCounter = 0;
	if(this->currentMenu != this->rootMenu)
	{
		this->currentMenu = static_cast<Menu*>(this->currentMenu->GetParent());
	}
	this->currentMenu->Select();
}

void MenuManager::home()
{
	this->rootMenu->index = 0;
	this->rootMenu->menuRowCounter = 0;
	this->currentMenu = this->rootMenu;
	this->currentMenu->Select();
}