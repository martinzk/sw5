#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H
#include <SdFat.h>
#include <SdFile.h>
#include <avr/pgmspace.h>
#include <NilRTOS.h>
/*House setup*/
#define NUMBER_OF_BUTTONS 5 //How many buttons are implemted in the house
#define NUMBER_OF_RELAYS 5 //How many relays are implemented in the house
#define UINT8MAX 255
/*--------------------------------------------------------*/

/*Motion setup*/
#define MOTION_DELAY 1500
/*--------------------------------------------------------*/


/*Button configurations*/
#define DEBOUNCE_TIME 65 
#define LONG_PUSH_START 500
#define LONG_PUSH_DURATION 2500
enum PushType {NO_PUSH = 0, SHORT_PUSH = 1, LONG_PUSH = 3, DOUBLE_PUSH = 2}; 
/*--------------------------------------------------------*/

/*DummyRelay configurations*/
//#define RELAY_DEBOUNCE_TIME 15 
/*--------------------------------------------------------*/


/*Sound sensor configurations*/
#define THRESHOLD 40 // at what level a clap is registered
#define MIDDLEVALUE 512 // middle of the range of analog values
#define NUMBEROFSAMPLES 128 // amount of readings each time
#define SOUND_SENSOR_DEBOUNCE_TIME 200
#define SECOND_CLAP_MAX_INTERVAL 1000
#define NUMBER_OF_SOUNDSENSORS 1
enum ClapType { NOCLAP = 0, CLAP = 1, DOUBLECLAP = 2 };
/*--------------------------------------------------------*/

/*RelayManager configurations*/

//How many relays can be turned on at the same time, due to electric current limitations
#define MAX_ACTIVE_RELAYS 6
#define OFF_TIME 200
/*--------------------------------------------------------*/

/*Event configurations*/
/*--------------------------------------------------------*/

/*Rule configurations*/
#define MAX_CONDITIONS 7
#define MAX_ACTIONS 7
#define MAX_RULES 15

/*--------------------------------------------------------*/
 
/*Simulation configurations */
#define MAX_NUMBER_OF_LEDS 3
/*Clock configurations*/
#define CLOCK_ADDRESS 0x68

/*Buzzer configurations*/
#define NUMBER_OF_BUZZERS 1

/*--------------------------------------------------------*/


/*Motionsensor configurations*/
#define NUMBER_OF_MOTIONSENORS 1

/*--------------------------------------------------------*/

/*RTC configurations*/
#define NUMBER_OF_RTCS 1

/*--------------------------------------------------------*/

/*Modes configurations*/
#define NUMBER_OF_MODES 4

/*--------------------------------------------------------*/



/*LCD configurations*/
/*Constants for Display && MenuItem */
#define MAX_NUMBER_OF_MenuItem_ITEMS 100
#define MAX_NUMBER_OF_SUB_MenuItemS 100
#define AD_KEYPAD_PIN A7
#define MAX_CHARACTER_ROWS 20
#define MAX_CHARACTER_COLUMNS 4
#define LCD_ADDRESS 0x27

/*--------------------------------------------------------*/

/*Webserver configurations*/
#define MAX_DATA_LENGTH 1000
#define MAX_KEY_LENGTH 60
#define TYPECHAR 0x74 //HEX value for t
#define LENGTHCHAR 0x6c //HEX value for l
#define DATACHAR 0x64 //HEX value for d
#define SEPERATE_SYMBOL 0x26 //HEX value for amperand
#define RESET_PIN 7 //Used to reset the arduino.
#define ETHERNET_CHIP 10
#define REQ_BUFFER_SIZE 80
#define MAC_ADDRESS_SIZE 6
/*--------------------------------------------------------*/
/* DataManager configurations*/
#define MAX_DATA_SIZE 2000
#define MAX_NUM_FILES 7
#define MAX_FILENAME_LENGTH 13 //Maximum allowed filename, according to SDFAT
#define CHIPSELECTSD 4

// SD chip select pin


/*--------------------------------------------------------*/

/* Constants used by DataManager and Websever*/
//file types
static const char buttonsFileName[] PROGMEM = "BL"; //button list
static const char relaysFileName[] PROGMEM = "REL"; //relay list
static const char alarmsFileName[] PROGMEM = "AL"; //ALARM list
static const char modesFileName[] PROGMEM = "ML"; //modes list
static const char motionFileName[] PROGMEM = "MSL"; //motionsensor list
static const char rulesFileName[] PROGMEM = "RUL"; //rule list
static const char soundsFileName[] PROGMEM = "SSL"; //sound sensor list
//Rule file for tempory storing rules on runtim
static const char memRulesFile[] PROGMEM = "MEMRULES"; //sound sensor list

static PGM_P const fileNames[MAX_NUM_FILES] PROGMEM =
{
	buttonsFileName, relaysFileName, modesFileName, motionFileName, rulesFileName, alarmsFileName, soundsFileName
};

//Sd object
static SdFat sd;

//Sd file for saving rules at runtime
static SdFile rulesFile;

static SEMAPHORE_DECL(i2cSem, 1);
static SEMAPHORE_DECL(SdSem, 1);
static SEMAPHORE_DECL(relaySem, 1);

#endif