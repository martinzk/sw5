#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <SdFat.h>
#include <SdFile.h>
#include <Button.h>
#include <TinyPacks.h>
#include <Configurations.h>
#include <EventContainer.h>
#include <RelayManager.h>
#include <Relay.h>
#include <StateContainer.h>
#include <MotionSensor.h>
#include <Alarm.h>
#include <mode.h>
#include <SdFatUtil.h>
#include <SoundSensor.h>
#include <avr/pgmspace.h>

class DataManager
{
	public:
		DataManager();
		void GetDataFromSD();
	private:
		void deserializeData(uint8_t *bytes, const uint16_t byteLength, char *type);
		void intToCharArray(uint8_t number, char* array);
		void clear(char* str, uint16_t length);
		void deserializeRules(uint8_t * packedRules, const uint16_t bufferLength, PackReader *reader);
		void deserializeEvent(PackReader *reader);
		void deserializeCondition(Rule *r, PackReader *reader);
		void deserializeAction(Rule *r, PackReader *reader);
		void deserialize(uint8_t *bytes, const uint16_t byteLength, PackReader *reader, char* type);
		bool isFileNameValid(char *fileName);
		bool StrContains(char *str, char *sfind);
		void getTypeAndInstantiateObject(uint8_t ID, uint8_t pin, char* type);
		EventContainer* ec;
		StateContainer* sc;
		RelayManager* rm;
		EventManager* em;
};

#endif