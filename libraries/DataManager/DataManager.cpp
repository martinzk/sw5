#include <DataManager.h>

#define MAX_FILE_COUNTER_SIZE 3
DataManager::DataManager()
{
	ec = EventContainer::GetEventContainer();
	sc = StateContainer::GetStateContainer();
	rm = RelayManager::GetRelayManager();
    em = EventManager::GetEventManager();
}



void DataManager::GetDataFromSD()
{

	if(!sd.begin(CHIPSELECTSD, SPI_HALF_SPEED)) sd.initErrorHalt();
	
    //Delete the runtime rules file.
    char rulesBufFileName[MAX_FILENAME_LENGTH];
    strcpy_P(rulesBufFileName, memRulesFile);
    if(sd.remove(rulesBufFileName))
    {
        Serial.println(F("Deleted the temp rule file"));
    }
    else
    {
        Serial.println(F("Error couldn't delete the temp rule!"));
    }

	//Got rules?
	bool rulesExists = false;

	char rulesFileNamebuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(rulesFileNamebuf, rulesFileName);

	//File object
	SdFile file;
	sd.vwd()->rewind();
	while (file.openNext(sd.vwd(), O_READ))
	{

		//Get name of file
		char fileName[MAX_FILENAME_LENGTH];
		file.getFilename(fileName);

		if(isFileNameValid(fileName))
		{
			uint8_t bytes[1000] = {0}; //Set values to zero.
			//Name of file was valid
			// Special case for rules, they need to be loaded into memory last

            if (!rulesExists && StrContains(fileName, rulesFileNamebuf))
            {
                Serial.println(F("INRULE"));
                rulesExists = true;
                file.close();
                delay(1000);
                continue;
            }

            // //Get length of data
            uint16_t dataLength = file.fileSize();
            // //Start reading
            file.read(bytes, dataLength);
            //deserialize the data bytes
            //Close file for memory
            file.close();

            deserializeData(bytes, dataLength, fileName);
            continue;
		}
		file.close();
	}

	Serial.println(F("done with everything except rules!"));
	delay(2000);


	//Any rules
    if (rulesExists)
    {
		char fileNumber[MAX_FILE_COUNTER_SIZE] = {0}; //Maximum number of files are 99;

		//Returns true if the file can be opened, otherwise returns false
		char buf[MAX_FILENAME_LENGTH] = {0};
	    strcpy_P(buf, rulesFileName);
		//Get length of file name
		uint8_t len = strlen(buf);
	    uint8_t counter = 1;
	    buf[len] = counter + '0';
	    Serial.println(buf);
	    //Open rule files 
		while(file.open(buf, O_READ))
		{
			//Data buffer
			uint8_t bytes[1000] = {0}; //Set values to zero.

			//Increment the counter
			counter++;
			Serial.println(buf);

			//clear the filenumber array
			clear(fileNumber, MAX_FILE_COUNTER_SIZE);

			//Insert the counter number into the char buf, which contains the file name
			intToCharArray(counter, fileNumber);

			//Insert the filenumber into the buf array, making it ready to open the next file
			uint8_t fileNumberLen = strlen(fileNumber);
			int8_t j = 0;
			for (uint8_t i = len; j < fileNumberLen && i < MAX_FILENAME_LENGTH; ++i, j++)
			{
				buf[i] = fileNumber[j];
			}

	        //Get length of data
	        uint16_t dataLength = file.fileSize();
	        //Start reading
	        file.read(bytes, dataLength);
	        file.close();

	        //deserialize the data bytes
	        deserializeData(bytes, dataLength, rulesFileNamebuf); 

		}

















    	// Serial.println(F("Rules exists"));
    	// uint8_t bytes[1000] = {0}; //Set values to zero.
    	// char buf[MAX_FILENAME_LENGTH] = {0};
     //    strcpy_P(buf, rulesFileName);

     //    if (!file.open(buf, O_READ)) 
     //    {
     //        sd.errorHalt("opening rules file for read failed");
     //    }

     //    //Get length of data
     //    uint16_t dataLength = file.fileSize();
     //    //Start reading
     //    file.read(bytes, dataLength);
     //    file.close();

     //    //deserialize the data bytes
     //    deserializeData(bytes, dataLength, buf);       
    }
}


void DataManager::intToCharArray(uint8_t number, char* array)
{
	uint8_t i = 0;


	while (number) { // loop till there's nothing left
    	array[i++] = number % 10 + '0'; // assign the last digit
    	number /= 10; // "right shift" the number
	}

	//The number are revesered now, e.g. 544 are 445.


	//Reverse the number again.
    char *df = array, *dr = array + i - 1;
    uint8_t j = i >> 1;

    while (j--)
    {
        char di = *df, dj = *dr;
        *df++ = dj, *dr-- = di; 
    }

}

void DataManager::clear(char* str, uint16_t length)
{
	for (int i = 0; i < length; ++i)
	{
		str[i] = 0;
	}
}

void DataManager::deserializeData(uint8_t *bytes, const uint16_t byteLength, char *type)
{
	PackReader reader;
    if(strcmp_P(type, rulesFileName) == 0)
    {
    	Serial.println(F("JEG ER I RULE"));
        deserializeRules(bytes, byteLength, &reader);
        Serial.println(F("Jeg er faerdig i rule"));
    }
    else
    {
    	//It's anything else, just deserialize
    	deserialize(bytes, byteLength, &reader, type);
    }
}

bool DataManager::StrContains(char *str, char *sfind)
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

bool DataManager::isFileNameValid(char *fileName)
{
    for (int i = 0; i < MAX_NUM_FILES; ++i)
    {
    	char buf[MAX_FILENAME_LENGTH] = {0};
    	strcpy_P(buf, (char*) pgm_read_word(&(fileNames[i])));
        if (StrContains(fileName, buf))
        {            
            return true;
        }
    }
    return false;
}


void DataManager::deserializeRules(uint8_t * packedRules, const uint16_t bufferLength, PackReader *reader)
{
	reader->setBuffer(packedRules, bufferLength);
	reader->next();
	//Open the list of rules
	reader->openList();
	while(reader->next())
	{
		Rule r;
		if(reader->openMap())
		{
			
			while(reader->next())
			{

				if(reader->match("e"))
				{
					deserializeEvent(reader);

				}
				else if(reader->match("c"))
				{

					deserializeCondition(&r, reader);
				}
				else if(reader->match("a"))
				{
					deserializeAction(&r, reader);
				}	
			}
		}
		//Add the rule to the file with rules
		 char memrulefilenameBuf[MAX_FILENAME_LENGTH];
  		strcpy_P(memrulefilenameBuf, memRulesFile);
	   	if (!rulesFile.open(memrulefilenameBuf, O_RDWR | O_CREAT | O_AT_END)) 
	   	{
	   		Serial.println("failed");
	   		return;
	   	}

		rulesFile.write(&r, sizeof(Rule));
		rulesFile.close();
		Serial.print(F("Size of ac"));
		Serial.println(r.numActions);
		reader->close();
	}
	reader->close();
}

void DataManager::deserializeEvent(PackReader *reader)
{
	uint8_t ID = 0;
	uint16_t eventType = 0;
	reader->openMap();
	while(reader->next())
	{
		if(reader->match("id"))
			ID = reader->getInteger();
		//Get eventType
		else if(reader->match("et"))
			eventType = reader->getInteger();
	}
	reader->close();
	//Subscribe the rule with it's bounded sensor id and eventtype
	Serial.print("etype");
	Serial.println(eventType);
	Subscriber s(ID, eventType);
	em->subscribe(s);
}

void DataManager::deserializeCondition(Rule *r, PackReader *reader)
{
	//Open list of conditions
	Serial.println(F("In Condition"));
	if(reader->openList())
	{
		while(reader->next())
		{
			uint8_t ID = 0, state = 0;
			if(reader->openMap())
			{
				while(reader->next())
				{
					if(reader->match("id"))
						ID = reader->getInteger();	        
				    else if(reader->match("s"))
			 	 		state = reader->getInteger(); 
				}
				//Add the condition to the list of conditions
				Condition c;
				Serial.println(state);
				c.StateComponent = sc->GetStateComponent(ID);
				c.SetState(state);
				r->AddCondition(c);
				reader->close();
			}
			
		}
		reader->close();
	}
	
}

void DataManager::deserializeAction(Rule *r, PackReader *reader)
{
	//Open list of actions
	reader->openList();
    while(reader->next())
    {
    	uint8_t ID = 0, state = 0; 
    	uint32_t delay = 0, duration = 0;
		reader->openMap();
		while(reader->next()) 
     	{
     	    if(reader->match("id"))
     	    	ID = reader->getInteger();
     	    else if(reader->match("s"))
     	    	state = reader->getInteger();
     	    else if(reader->match("w"))
     	    	delay = reader->getInteger();
     	    else if(reader->match("d"))
     	    	duration = reader->getInteger();
     	}
     	reader->close();
     	//Add action to the rule
 		Action a(state, delay, duration);
 		a.StateComponent = sc->GetStateComponent(ID);

 		r->AddAction(a);

    }
    reader->close();
}

void DataManager::deserialize(uint8_t *bytes, const uint16_t byteLength, PackReader *reader, char* type)
{
	reader->setBuffer(bytes, byteLength);

	reader->next();
	reader->openList();
	while(reader->next())
	{
		//Define properties
		uint8_t ID = 0, pin = 0;

		reader->openMap();
		while(reader->next())
		{
		    if(reader->match("id")) 
		    	ID = reader->getInteger();
		    else if (reader->match("p")) 
		    	pin = reader->getInteger();
		}
		reader->close();
		Serial.println(ID);
		Serial.println(pin);

		//an objects data is deserialized
		//Determine which type of object and instantiate it.

		getTypeAndInstantiateObject(ID, pin, type);
	}
	reader->close();
}

void DataManager::getTypeAndInstantiateObject(uint8_t ID, uint8_t pin, char* type)
{
	/*Get all file names from program memory*/
	char relaysFileNameBuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(relaysFileNameBuf, relaysFileName);

	char buttonsFileNameBuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(buttonsFileNameBuf, buttonsFileName);

	char modesFileNameBuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(modesFileNameBuf, modesFileName);

	char motionFileNameBuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(motionFileNameBuf, motionFileName);

	char alarmsFileNameBuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(alarmsFileNameBuf, alarmsFileName);

	char soundsFileNameBuf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(soundsFileNameBuf, soundsFileName);
    /*											*/

	if(StrContains(type, relaysFileNameBuf))
    {
    	Serial.println(F("JEG ER I RELAY"));
    	Relay r(ID, pin, 200); //Todo 200-> get from ukuk
    	rm->AddRelay(r);
		//Load into sc container
		Relay* rptr = rm->GetRelayPointerFromID(ID);

		sc->AddStateComponent(rptr);
        Serial.println(F("Jeg er faerdig med relay"));
    }
    else if (StrContains(type, buttonsFileNameBuf))
    {
    	Serial.println(F("Jeg er i button"));
        Button b(ID, pin);
        ec->AddButton(b);
        Serial.println(F("Jeg er faedig med button"));

    }
    else if(StrContains(type, modesFileNameBuf))
    {
    	Mode m(ID);
    	sc->AddMode(m);
    }
    else if(StrContains(type, motionFileNameBuf))
  	{
  		MotionSensor ms(ID, pin);
  		ec->AddMotionSensor(ms);
  	}
    else if(StrContains(type, alarmsFileNameBuf))
	{
		Alarm a(ID, pin);
		sc->AddAlarm(a);
	}
    else if(StrContains(type, soundsFileNameBuf))
	{
		SoundSensor ss(ID, pin);
		ec->AddSoundSensor(ss);
	}
}