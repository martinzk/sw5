#include <SoundSensor.h>
#include <Configurations.h>
#include <SdFat.h>
#include <IEventComponent.h>
#include <Event.h>
#include <Rule.h>
#include <IStateComponent.h>
//#include <Algorithm2.h>
//#include <Algorithm3.h>
SoundSensor soundSensor(1, A0);

void setup()
{
	Serial.begin(9600);

}

void loop()
{	
	ClapType clap = soundSensor.GetClapType();
	if(clap == 2)
	{
		Serial.println("doubleclap");
	}
	// if(clap == 1)
	// {
	// 	Serial.println("singleclap");
	// }
}