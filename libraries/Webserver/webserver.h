#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Configurations.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SdFat.h>
#include <StateContainer.h>
#include <IStateComponent.h>
#include <Relay.h>
#include <RelayManager.h>



class Webserver 
{
  public:
    Webserver();
    void Init(IPAddress ip, IPAddress gateway, IPAddress subnet, byte* mac, uint8_t port);
    void ListenForClients();

  private:
    bool receivedRequest;
    EthernetServer* server;
	char HTTP_req[REQ_BUFFER_SIZE];
    
	void getNewConfigRequest(EthernetClient* client);
	void changeState(EthernetClient* client);
    void RemoveAllFilesFromSD();

    void sendManageData(EthernetClient* client);
    void getRequestHeader(EthernetClient *client);
    bool isGetRequest();
    void getFileName(EthernetClient *client, char * fileName);
    bool isFileNameValid(char *fileName);
    uint16_t getDataLength(EthernetClient *client);
    bool isDataLengthValid(uint16_t dataLength);
    bool getDataAndStore(EthernetClient *client, char *fileName);
    void resetArd();
	uint8_t getByteValue(const char c);
    void HttpRequestOKStatusResponse(EthernetClient* client);
	void errorBadRequest(EthernetClient* client);
	void errorInternalServerError(EthernetClient* client);
    void switchState(EthernetClient* client);
    bool StrContains(char* str, char* sfind);
    void StrClear(char* str, char length);
    void readKeyStr(char *key, EthernetClient* client);
    void sendConfig(EthernetClient* client);
    void getIdAndState(char* http_req, uint8_t* id, uint8_t* state);
    void getId(char* http_req, uint8_t* id);
    uint8_t charArrayToUInt8(char* array);
};

#endif