#include <Configurations.h>
#include <webserver.h>
#include <SPI.h>
#include <Ethernet.h>
#include <StateContainer.h>
#include <IStateComponent.h>

#include <SdFat.h>
Webserver wb;

void setup()
{
	Serial.begin(9600);
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	IPAddress ip( 192, 168, 87, 116 );
	IPAddress gateway( 192,168,87,1 );
	IPAddress subnet( 255,255,255,0 );
	wb.Init(ip, gateway, subnet, mac, 80);

}

void loop()
{
	wb.ListenForClients();
}