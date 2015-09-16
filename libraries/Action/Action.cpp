#include <Action.h>

Action::Action(char* name, void (*action)())
{
	this->action = action;
	this->name = name;
}

void Action::Select()
{
	this->action();
}
char* Action::GetType()
{
	return "Action";
}
char* Action::GetName()
{
	return this->name;
}

void Action::SetParent(IMenuItem* parent)
{
    this->parent = parent;
}
