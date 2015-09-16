#include <display.h>
#define MAX_CHARACTER_ROWS 20 //Todo define these in config...
#define MAX_CHARACTER_COLUMNS 4
#define LCD_ADRESS 0x27

//init lcd display
LiquidCrystal_I2C lcd(LCD_ADRESS, MAX_CHARACTER_ROWS, MAX_CHARACTER_COLUMNS);

bool Display::instanceFlag = false;
Display* Display::display = NULL;

Display* Display::GetDisplay()
{
    if (!instanceFlag)
    {
        display = new Display();
        instanceFlag = true;
        return display;
    }
    return display;
}

Display::Display()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    this->CurrentRow = 0; 
}

/***********Public functions *********/

void Display::Clear()
{
    lcd.clear();
    CurrentRow = 0;
}

void Display::PrintDate(DateTime dt)
{

    lcd.setCursor(0,0);
    lcd.print(F("Date: "));
    lcd.print(dt.date);
    lcd.print(F("/"));
    lcd.print(dt.month);
    lcd.print(F("/"));
    isAboveNine(dt.year);
    lcd.print(dt.year);
    lcd.setCursor(0,1);
    lcd.print(F("Time:"));
    isAboveNine(dt.hour);
    lcd.print(dt.hour);
    lcd.print(F(":"));
    isAboveNine(dt.minute);
    lcd.print(dt.minute);
    lcd.print(F(":"));
    isAboveNine(dt.second);
    lcd.print(dt.second);
    
}
void Display::PrintMemory(long mem)
{
    lcd.setCursor(0,2);
    lcd.print(F("Free memory: "));
    if (mem < 1000)
    {
        lcd.print(mem);
        lcd.print(F(" b"));
    }
    else 
    {
        lcd.print(mem);
        lcd.print(F("b"));
    }
}

/***********Private functions *********/
void Display::updateCurrentRow()
{
    if (CurrentRow < 3) {
        CurrentRow++;
    }
    else
        CurrentRow = 0;
}
void Display::isAboveNine(uint8_t input)
{
    if (input < 10)
        lcd.print(0);
}