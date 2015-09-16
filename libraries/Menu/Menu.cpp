#include <Menu.h>
#include <Configurations.h>
//SumMenuItem
Menu::Menu(char* name): IMenuItem() 
{
	if(strlen(name) > MAX_CHARACTER_ROWS-1)
	{
		name = "Error text too long";
	}
	this->name = name;
    this->menuSize = 0;
    this->index = 0;
    this->menuRowCounter = 0;
    this->display = Display::GetDisplay();
}

Menu::Menu(): IMenuItem()
{
    
}

char* Menu::GetType()
{
    return "Menu";
}

void Menu::Select()
{   
    this->display->Clear();
    this->Show();
}

IMenuItem* Menu::GetMenuChild()
{
    return children[this->index];
}

void Menu::Show()
{
    for (int i = this->menuRowCounter; i < this->menuSize && i < 4 + menuRowCounter; ++i)
    {
    	//Check if the current item in the MenuItem is chosen
        //If it is, then add a star
        //Serial.println(i); 
        if (i == this->index)
        {  
        	char currentMenuText[strlen(children[i]->GetName())+2];
        	strncpy(currentMenuText, children[i]->GetName(), strlen(children[i]->GetName()));

	       	append(currentMenuText, '*', strlen(children[i]->GetName()));
            this->display->PrintLeft(currentMenuText);
        }

        else 
        {
            this->display->PrintLeft(children[i]->GetName());
        }
    }
}

void Menu::append(char *s, char c, int len) 
{
    *(s + len) = '*'; 
    *(s + len + 1) = '\0';
}

char* Menu::GetName()
{
	return name;
}

int Menu::GetMenuSize() 
{
    return this->menuSize;
}

void Menu::Add(IMenuItem* menuItem)
{
    children[menuSize] = menuItem;
    children[menuSize]->SetParent(this); 
    this->menuSize++;
}

IMenuItem* Menu::GetParent()
{
	return this->parent;
}

void Menu::SetParent(IMenuItem* parent)
{
    this->parent = parent;
}
