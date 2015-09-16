#include <webserver.h>
#include <Rule.h>
Webserver::Webserver(){}


void Webserver::Init(IPAddress ip, IPAddress gateway, IPAddress subnet, byte* mac, uint8_t port)
{
    //Configure the resetpin.
    digitalWrite(RESET_PIN, HIGH);
    pinMode(RESET_PIN, OUTPUT);

    delay(1000);
    // Set SD and Ethernet pins.
    pinMode(CHIPSELECTSD,OUTPUT);
    digitalWrite(CHIPSELECTSD,HIGH);
    pinMode(ETHERNET_CHIP,OUTPUT);
    digitalWrite(ETHERNET_CHIP,HIGH);

 
    server = new EthernetServer(port); //instantiate ethernet device
    // Initialize SdFat or print a detailed error message and halt
    if (!sd.begin(CHIPSELECTSD, SPI_HALF_SPEED)) sd.initErrorHalt();

    // start the Ethernet connection and the server:
    Ethernet.begin(mac, ip, gateway, subnet);
    // disable w5100 SPI so SD SPI can work with it
    digitalWrite(ETHERNET_CHIP,HIGH);
    delay(2000);
    
    server->begin(); //listen for clients
}

void Webserver::ListenForClients()
{
  	// listen for incoming clients
    EthernetClient client = server->available();
    if(!client) 
    {
        return;
    }

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) 
    {
        bool newConfigRequest = false, changeStateRequest = false, switchStateRequest = false;
        while(client.available()) 
        {
            //Post requests..
            char c = client.read();           
            /* if you've gotten to the end of the line (received a newline
               character) and the line is blank, the http request has ended,
             so you can send a reply */ 
            if (c == '\n' && currentLineIsBlank) 
            {
                if(newConfigRequest)
                {
                    // for (int i = 0; i < 7; ++i)
                    // {
                    //     thread_ref_t tr = &nil.threads[i];
                    //     tr = nil.idlep;
                    // }
                    
                    getNewConfigRequest(&client);
                }
            }
            else if (c != '\n' && !receivedRequest)
            {
                //Get the first line of the request header.
                getRequestHeader(&client);

                //Check if the request is a GET call.
                if(isGetRequest())
                {                  
                    //Check if it is a valid get request
                    if(StrContains(HTTP_req, "ManageData"))
                    {
                        sendManageData(&client);
                    }
                    else if(StrContains(HTTP_req, "ChangeState"))
                    {
                        changeState(&client);
                    }
                    else if(StrContains(HTTP_req, "SwitchState"))
                    {
                        switchState(&client);
                    }
                    // else if(StrContains(HTTP_req, "GetConfig"))
                    // {
                    //     sendConfig(&client);
                    // }
                }
                //Else it is a post request
                else
                {
                    if(StrContains(HTTP_req, "NewConfig"))
                        newConfigRequest = true;
                }
                receivedRequest = true;
            }  
            else if (c == '\n') 
            {
                // you're starting a new linea
                currentLineIsBlank = true;
            }
            else if (c != '\r')
            {
                // you've gotten a character on the current line
                currentLineIsBlank = false;
            }   
        }
    }

    //Reset values.
    receivedRequest = false;
    StrClear(HTTP_req, REQ_BUFFER_SIZE);
    client.stop();
}

void Webserver::getNewConfigRequest(EthernetClient* client)
{
    Serial.println("getnewConfig");
    //Close the rule "memory" file, since we're not gonna use it any more in the session
    //rulesFile.close();

    char fileName[MAX_FILENAME_LENGTH] = {0}; //Set values to zeros.
    uint16_t dataLength = 0;
    //Start reading the post data.

    //Remove all files from the SD.
    RemoveAllFilesFromSD();
    while(client->available())
    {
        //Get which Type of data we're getting                      
        int c = client->read();

        Serial.write(c);
        // If the first char of the request isn't t, return error.
        if((c == TYPECHAR))
        {              
            //get filename from request
            getFileName(client, fileName);

            //Check if filename from the request is valid
            if(!isFileNameValid(fileName) && !client->available())
            {
                errorBadRequest(client);
                return;
            }
                            
            //read the next incoming char.  
            c = client->read();
            
            //Check for the length char. 
            if(c == LENGTHCHAR)
            {
                //Get length of data..
                dataLength = getDataLength(client);

                //Check is length is valid, and there is more data to read
                if(!isDataLengthValid(dataLength) && !client->available())
                {
                    errorBadRequest(client);
                    return;
                }

                //Read the next incoming data
                c = client->read();

                if(c == DATACHAR)
                {
                    //Retrive data from request and store it on the SDCard
                    //If it fails, there's an error in the request.
                    if(!getDataAndStore(client, fileName))
                    {
                        errorBadRequest(client);
                        return;
                    }
                }
                else
                {
                    errorBadRequest(client);
                    return;
                }
            }
            else
            {
                errorBadRequest(client);
                return;
            }

        }
        else
        {
            errorBadRequest(client);
            return;
        }
    }

    //Everything worked as expected, return ok.
    HttpRequestOKStatusResponse(client);
    //Reboot the arduino
    resetArd();
}

void Webserver::RemoveAllFilesFromSD()
{
    SdFile file;
    //Revind in voloume
    
    sd.vwd()->rewind();
    //Iterate through all files.
    while (file.openNext(sd.vwd(), O_WRITE))
    {
        file.remove();
        //Close making it ready for the next file..
        file.close();
    }
}

void Webserver::switchState(EthernetClient* client)
{
	uint8_t id = 0;
    getId(HTTP_req, &id);

    if (id == 0) // ID wasn't found
    {
    	errorBadRequest(client);
        return;
    }

    RelayManager* rm = RelayManager::GetRelayManager();
    Relay* r = rm->GetRelayPointerFromID(id);

    r->SetAssumingState(!r->GetCurrentState());
    HttpRequestOKStatusResponse(client);
}


void Webserver::getId(char* http_req, uint8_t* id)
{
    uint8_t idFound = 0;
    uint8_t index = 0;
    uint8_t len;

    char* idToken = "id=";

    char idChars[4] = {0};
    int i = 0;
 
    len = strlen(http_req);
   
   	StrClear(idChars, 4);
    while (index < len)
    {
        if (http_req[index] == idToken[idFound])
        {
            idFound++;
            if (strlen(idToken) == idFound)
            {  
                i = 0;
                //Skip equal sign
                index++;
                while (http_req[index] != '&' && index < len && i < 4 && http_req[index] >= '0' && http_req[index] <= '9') // concatenate the numbers
                {
                    idChars[i++] = http_req[index++];
                }

                //Make sure that we have met the & symbol
                while (http_req[index++] != '&') {}
                *id = charArrayToUInt8(idChars);                
            }    
        }
        index++;
    }
}

void Webserver::changeState(EthernetClient* client)
{
	uint8_t id = 0;
	uint8_t state = 0;
    getIdAndState(HTTP_req, &id, &state);

    if (id == 0)  // ID or state wasn't found
    {
    	errorBadRequest(client);
        return;
    }

    StateContainer* sc = StateContainer::GetStateContainer();
    IStateComponent* isc = sc->GetStateComponent(id);

    isc->Insert(state, 0, 0);

    //Request went ok
    HttpRequestOKStatusResponse(client);
}


void Webserver::readKeyStr(char *key, EthernetClient* client)
{
    uint8_t i = 0;
    if(client->peek() == SEPERATE_SYMBOL)
        client->read(); //Skip it.
    while(client->available() && i < MAX_KEY_LENGTH)
    {
        char c = client->read();
        if(c != '=')
            key[i++] = c;
        else
            break;
    }
}

void Webserver::sendConfig(EthernetClient* client)
{

    // client->println(F("HTTP/1.1 200 OK"));
    // client->print(F("Access-Control-Allow-Origin: "));
    // client->println(F("*"));
    // client->print(F("Content-Type: "));
    // client->println(F("text/plain"));

    // int data = 0;

    // if (!sd.begin(CHIP_SELECT, SPI_HALF_SPEED)) sd.initErrorHalt();

    // SdFile file;
    // HttpRequestOKStatusResponse(client);
    // while (file.openNext(sd.vwd(), O_READ)) 
    // {
    //     while ((data = file.read()) != -1) { client->print(data); Serial.print(data); }
    //     client->print('\n');
    //     Serial.println();
    //     file.close();
    // }    
    // client->stop();
}

void Webserver::sendManageData(EthernetClient* client)
{
    StateContainer* container = StateContainer::GetStateContainer();
    client->println(F("HTTP/1.1 200 OK"));
    client->print(F("Access-Control-Allow-Origin: "));
    client->println(F("*"));
    client->print(F("Content-Type: "));
    client->println(F("application/json"));
    client->println();
    client->print(F("["));

    
    for (int i = 0; i < container->GetLength(); ++i)
    {
        client->print(F("{ \"id\":"));
        client->print(container->stateComponents[i]->GetID());
        client->print(F(", \"s\":"));
        client->print(container->stateComponents[i]->GetCurrentState());
        client->print(F("}"));
        if (i + 1 != container->GetLength())
            client->print(F(","));
    }
    
    client->print(F("]"));
    client->stop();
}

void Webserver::getFileName(EthernetClient* client, char * fileName)
{
    client->read(); //Skip equal sign
    uint8_t i = 0;
    while(client->available())
    {
        fileName[i++] = client->read();
        //Check for data sepration symbol
        if((int)client->peek() == SEPERATE_SYMBOL)
        {
            //skip seperate symbol
            client->read();
            break;
        }
    }
    //Terminate string
    fileName[i] = '\0';                            
}

bool Webserver::isFileNameValid(char *fileName)
{
    for (int i = 0; i < MAX_NUM_FILES; ++i)
    {
        char buf[MAX_FILENAME_LENGTH] = {0};
        strcpy_P(buf, (PGM_P)pgm_read_word(&(fileNames[i])));
        if (StrContains(fileName, buf))
        {            
            return true;
        }
    }
    return false;
}


uint16_t Webserver::getDataLength(EthernetClient* client)
{   
    //Get the length of the incoming data.
    client->read(); //Skip equal sign
    uint16_t dataLength;
    while(client->available())
    {
        //Read size
        dataLength = client->parseInt();
        //Check for data sepration symbol
        if((int)client->peek() == SEPERATE_SYMBOL)
        {
            //skip seperate symbol
            client->read();
            break;
        }
    }
    return dataLength;
}

bool Webserver::isDataLengthValid(uint16_t dataLength)
{
    //Check for the allowed maximum 
    if (dataLength > MAX_DATA_LENGTH)
    {
        //Error can't handle size of data
        return false;
    }
    return true;
}



bool Webserver::getDataAndStore(EthernetClient* client, char *fileName)
{
    //Serial.write("Reading data: ");
    client->read(); //Skip equal sign

    
    //Open and create a new file
    SdFile dataFile;

    Serial.println(fileName);
    if (!dataFile.open(fileName, O_RDWR | O_CREAT | O_AT_END)) 
    {
        //Internal error.
        errorInternalServerError(client);
        sd.errorHalt("opening file for write failed");
    }
    bool gotByte = true;
    uint8_t byteValue = 0;
   
    while(client->available())
    {
        char c = client->read();
        byteValue *= 16;

        byteValue += getByteValue(c);
        gotByte=!gotByte;
        if(gotByte)
        {                              
            //Write byte to file.
            //Serial.print(byteValue, HEX);
            dataFile.write(byteValue);
            dataFile.sync();
            byteValue=0;
        }

        //Check for data sepration symbol
        if((int)client->peek() == SEPERATE_SYMBOL)
        {
            //skip seperate symbol
            client->read();

            //Seprate symbol, means more data, is it of the correct syntax?
            if(!(int)client->peek() == TYPECHAR)
            {
                dataFile.close();
                return false;
            }
            //If it is, just break the while loop.
            break;
        }
    }

    //Close the datafile.
    dataFile.close();
}


void Webserver::resetArd()
{
    Serial.println(F("Resetting arduino..."));
    digitalWrite(RESET_PIN, LOW);
}

uint8_t Webserver::getByteValue(const char c)
{
    if( (c >= '0') && (c <= '9') )
        return (c - '0');
    else if( (c >= 'a') && (c <= 'f') )
        return (c - 'a' + 10);
    else if( (c >= 'A') && (c <= 'F') )
        return (c - 'A' + 10);
}

void Webserver::HttpRequestOKStatusResponse(EthernetClient* client)
{
    // send a standard http response header
    client->println("HTTP/1.1 200 OK");
    client->print(F("Access-Control-Allow-Origin:"));
    client->print(F("*"));
    client->println();
    client->stop();
}

void Webserver::errorBadRequest(EthernetClient* client)
{
    //Error 400, request does not have the corret syntax
    client->println(F("HTTP/1.0 400 Bad Request"));
    client->flush();
    client->stop();
}

void Webserver::errorInternalServerError(EthernetClient* client)
{
    //Error 500, Internal Server Error
    client->println(F("HTTP/1.0 500 Internal Server Error"));
    client->stop();
}


void Webserver::getRequestHeader(EthernetClient* client)
{
    char c = 0;
    int req_index = 0;
    while(client->available() && c != '\n')
    {   
        c = client->read();

        if(req_index < (REQ_BUFFER_SIZE))
        {
            HTTP_req[req_index++] = c;
        }
    }
    receivedRequest = true;
}

uint8_t Webserver::charArrayToUInt8(char* array)
{
    uint8_t multiplier = 1;
    uint8_t number = 0;
    uint8_t len = strlen(array);
 
    for (int i = len - 1; i >= 0; --i)
    {
        number += (array[i] - '0') * multiplier;
        multiplier = multiplier * 10;
    }
    return number;
}
 
void Webserver::getIdAndState(char *http_req, uint8_t* id, uint8_t* state)
{
    uint8_t idFound = 0;
    uint8_t stateFound = 0;
    uint8_t index = 0;
    uint8_t len;
    char* idToken = "id=";
    char* stateToken = "s=";


    char stateChars[4] = {0};
    char idChars[4] = {0};
    int i = 0;
 
    len = strlen(http_req);
   
    while (index < len)
    {
        if (http_req[index] == idToken[idFound])
        {
            idFound++;
            if (strlen(idToken) == idFound)
            {  
                i = 0;
                //Skip equal sign
                index++;
                while (http_req[index] != '&' && index < len && i < 4 && http_req[index] >= '0' && http_req[index] <= '9') // concatenate the numbers
                {
                    idChars[i++] = http_req[index++];
                }
                //Make sure that we have met the & symbol
                while (http_req[index++] != '&') {}
                *id = charArrayToUInt8(idChars);                
            }    
        }
        if (http_req[index] == stateToken[stateFound])
        {
            stateFound++;
            if (strlen(stateToken) == stateFound)
            {  
                i = 0;
                //Skip equal sign
                index++;
                while (index < len && i < 4 && http_req[index] >= '0' && http_req[index] <= '9') // concatenate the numbers
                {
                    stateChars[i++] = http_req[index++];
                }
                //Make sure that we have met the & symbol
                *state = charArrayToUInt8(stateChars);                
            }    
        }
        index++;
    }
}

bool Webserver::isGetRequest()
{
    if(StrContains(HTTP_req, "ET"))
    {
        return true;
    }
    return false;  
}

// searches for the string sfind in the string str
// returns true if string found
// returns false if string not found
bool Webserver::StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);

    if (strlen(sfind) > len) 
    {
        return false;
    }
    while (index < len) 
    {
        if (str[index] == sfind[found]) 
        {
            found++;
            if (strlen(sfind) == found) 
            {
                return true;
            }
        }
        else 
        {   
            found = 0;
        }
        index++;
    }

    return false;
}

void Webserver::StrClear(char *str, char length)
{
  for (int i = 0; i < length; i++) 
  {
    str[i] = 0;
  }
}