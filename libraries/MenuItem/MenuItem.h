#ifndef ____IMenuItem__
#define ____IMenuItem__
#include <Arduino.h>
#include <Configurations.h>

class IMenuItem
{
public:
    IMenuItem();
    virtual char* GetName() = 0;
    virtual void Select() = 0;
    virtual void SetParent(IMenuItem* parent) = 0;
    virtual char* GetType() = 0;
};

#endif