#define MAX_FILENAME_LENGTH 13
#define MAX_FILE_COUNTER_SIZE 3







void setup()
{
	//Read rules from multiple files
	SdFile file;

	//Char array for holding the the file number
	char fileNumber[MAX_FILE_COUNTER_SIZE] = {0}; //Maximum number of files are 99;

	//Returns true if the file can be opened, otherwise returns false
	char buf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(buf, rulesFileName);

	//Get length of file name
	uint8_t len = strlen(buf);
    uint8_t counter = 1;
    //Open rule files 
	while(file.open(buf, O_READ))
	{
		//Increment the counter
		counter++;
	

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
	}
	Serial.println("End");

}

void loop() {}


void intToCharArray(uint8_t number, char* array)
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

void clear(char* str, uint16_t length)
{
	for (int i = 0; i < length; ++i)
	{
		str[i] = 0;
	}
}