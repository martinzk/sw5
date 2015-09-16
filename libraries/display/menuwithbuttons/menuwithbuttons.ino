#include <Action.h>
#include <ADButtons.h>
#include <display.h>
#include <LiquidCrystal_I2C.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuManager.h>
#include <setupConstants.h>
#include <Wire.h>

void TurnOffLight()
{
  digitalWrite(13, LOW);
}
void TurnOnLight()
{
  digitalWrite(13, HIGH);
}

void TurnOffAllLights()
{
    //for light in relays
    //light.turnOff()
}
void setup(){  
  pinMode(13, OUTPUT);

}
bool flag = true;

void loop(){
  if (flag == true)
  {
    //Main 
    Menu Main("main"); 
    Menu lightMenu("Light Control");
    Menu rooms("Rooms");
    //Decl. Rooms
    Menu room1("Bedroom");
    Menu room2("Markus's room");    
    Menu room3("Office");
    Menu room4("Outside");
    Menu room5("Bathroom");
    Menu room6("Gang");
    Menu room7("Hall");
    Menu room8("Living room");
    Menu room9("Kitchen");
    //Add rooms to main room menu
    rooms.Add(&room1);
    rooms.Add(&room2);
    rooms.Add(&room3);
    rooms.Add(&room4);
    rooms.Add(&room5);
    rooms.Add(&room6);
    rooms.Add(&room7);
    rooms.Add(&room8);
    rooms.Add(&room9);

    //Bind actions to rooms
    Action TurnOff("Turn off lights", TurnOffLight);
    Action TurnOn("Turn on lights", TurnOnLight);
    Action TurnAllOff("Turn Off all lights", TurnOffAllLights);

    //Bind actions to rooms
    room1.Add(&TurnOff);
    room1.Add(&TurnOn);
    room2.Add(&TurnOff);
    room2.Add(&TurnOn);
    room3.Add(&TurnOff);
    room3.Add(&TurnOn);
    room4.Add(&TurnOff);
    room4.Add(&TurnOn);
    room5.Add(&TurnOff);
    room5.Add(&TurnOn);
    room6.Add(&TurnOff);
    room6.Add(&TurnOn);
    room7.Add(&TurnOff);
    room7.Add(&TurnOn);
    room8.Add(&TurnOff);
    room8.Add(&TurnOn);
    room9.Add(&TurnOff);
    room9.Add(&TurnOn);
    
    //Add all stuff together
    lightMenu.Add(&rooms);
    lightMenu.Add(&TurnAllOff);
    Main.Add(&lightMenu);
    MenuManager* mm;
    mm = MenuManager::GetMenuManager();
    mm->AddRootMenu(&Main);
    mm->SelectPush();

    flag = false; 
    
  }

}


