#include <Menu.h>
#include <MenuManager.h>
#include <Action.h>
#include <ADButtons.h>
#include <Button.h>
#include <ButtonManager.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <display.h>
#include <RelayManager.h>
#include <Configurations.h>

//Buttons
ButtonManager buttonManager;
Menu Main; 
MenuManager *mm = MenuManager::GetMenuManager();
RelayManager *rm = RelayManager::GetRelayManager();
int numButtons = 16;
int numRelays = 16;

  //Get list of Relay pins, array
  int relayPin[] = 
  {
    52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22
  };

/*
 * Example to demonstrate thread definition, semaphores, and thread sleep.
 */
#include <NilRTOS.h>

// Declare and initialize a semaphore for limiting access to a region.
SEMAPHORE_DECL(sem, 1);

NIL_WORKING_AREA(waThread1, 1024);

// Declare the thread function for thread 1.
NIL_THREAD(Thread1, arg) {
  while(true){
    // statement
    nilSemWait(&sem);
    buttonManager.ListenForButtonPush();
    rm->ListenForReadyRelays();
    nilSemSignal(&sem);
    nilThdSleepMilliseconds(10);
  }
}


NIL_WORKING_AREA(waThread2, 2048);


//LiquidCrystal_I2C d(0x27,20,4);

// Declare the thread function for thread 1.
NIL_THREAD(Thread2, arg) {
  while(true){
    // statement
    mm->SelectPush();
  }
}


//------------------------------------------------------------------------------

void SetOneButton()
{
  nilSemWait(&sem);

  //Get list of buttonPins from eeprom/Webserver
  int buttonPins[] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23};
  //Reset buttons in button manager, if any. 
  buttonManager.ResetButtonManager();
  
  for(int i=0; i<16; i++){
        buttonManager.Add(Button(buttonPins[i]));
        pinMode(buttonPins[i], INPUT_PULLUP);
    }

  ResetButtons();
  //Toggle every relay
  for(int i=0; i<numRelays; i++)
  {
      //Turn relay on
      //Wait for a buttonPush
      ToggleRelay(relayPin[i]);
      RegisterButton(relayPin[i]);
  }
      
  

  //setButtonArraySize(buttons);

  nilSemSignal(&sem);
}

void RegisterButton(int relayPinId)
{
    bool isRegistered = false;
    // mens ingen knap er blevet trykket
    while(!isRegistered)
    {
      // gennemløb alle knapper
      for(int i = 0; i < buttonManager.Size(); i++)
      {
          if(buttonManager.buttons[i].GetPush() == SHORT_PUSH)
          {
            if(buttonManager.buttons[i].RelayPresent(relayPinId))
            {

              isRegistered = true;
              ToggleRelay(relayPinId);
              break;
            }
            buttonManager.buttons[i].AddRelay(relayPinId);
          }
      }
    }
}

// void setButtonArraySize()
// {
//   for(int i=0; i<numButtons; i++){
//       int numberOfRelays = buttons[i].GetNumberRelays();
//       Button button(buttons[i].inPin, numberOfRelays);

//       for(int j=0; j<numberOfRelays; j++)
//       {
//           button.AddRelay(buttons[i].GetRelay(j));    
//       }
//       buttonManager.Add(button);
//   }
// }

void ResetButtons()
{
    for(int i=0; i<numButtons; i++)
    {
      buttonManager.buttons[i].ResetRelays();
    }
}



void ToggleRelay(int relayPin)
{

  digitalWrite(relayPin, LOW);
  unsigned long timeOn = millis();
  while(millis() - timeOn <= 200)
  {
  }
  digitalWrite(relayPin, HIGH);

  //Todo Fjern busyWait
}

//------------------------------------------------------------------------------
/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 * 
 * These threads start with a null argument.  A thread's name may also
 * be null to save RAM since the name is currently not used.
 */
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY("thread1", Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY("thread2", Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()
//------------------------------------------------------------------------------
void setup() {
  for(int i=0; i<numRelays; i++){
      rm->AddRelay(Relay(relayPin[i]));
  }
  Main = Menu("Main");
  Menu m1 = Menu("Hej");
  Action buttonSetup("Button Setup", &SetOneButton);
  m1.Add(&buttonSetup);
  Main.Add(&m1);
  mm->AddRootMenu(&Main);
  //SetOneButton();
  /*
  buttonManager.Add(button_A);
  buttonManager.Add(button_B);
  buttonManager.Add(button_C);
  buttonManager.Add(button_D);
  buttonManager.Add(button_E);
  buttonManager.Add(button_F);
  buttonManager.Add(button_G);
  buttonManager.Add(button_H);
  buttonManager.Add(button_I);
  buttonManager.Add(button_J);
  buttonManager.Add(button_K);
  buttonManager.Add(button_L);
  buttonManager.Add(button_M);
  buttonManager.Add(button_N);
  buttonManager.Add(button_O);
  buttonManager.Add(button_P);
  */

  //delay(3000);
  // Start Nil RTOS.
  nilSysBegin();
}
//------------------------------------------------------------------------------
// Loop is the idle thread.  The idle thread must not invoke any 
// kernel primitive able to change its state to not runnable.
void loop() {
  // Not used.
}

