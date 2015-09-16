#include <GapContainer.h>

void setup() 
{
    Serial.begin(9600);
    //getFreeMemory();
}

uint32_t i = 0; 
uint32_t stateTimes = 0; 
GapContainer rt;
void loop() 
{
    if(i == 0)
    {
        rt.AddGap(1000, 1000, 1);
        rt.AddGap(2000, 2000, 2);
        rt.AddGap(4000, 3000, 3);
        rt.AddGap(7000, 4000, 4);
        rt.AddGap(11000, 5000, 5);
        rt.AddGap(16000, 6000, 6);
        while(true){
            delay(100); // statement
            Serial.println(rt.GetState());
        }
        /*
  for(i = 0; i<10000; i++)
         {
         TimeGap rg;
         rg.duration = (random(0, 100) * 1000);
         delay(7);
         rg.state = random(0, 2);
         delay(4);
         inputWait = (random(0, 100) * 1000);
         if(i % 1000 == 0)
         {
         getFreeMemory();
         }
         rt.AddGap(rg, inputWait);
         }
         getFreeMemory();
         
         //getFreeMemory();
         */
        Serial.println("LOOP IS DONE");
        /*
    uint32_t inputWait = 0; 
         
         TimeGap rg1;
         rg1.duration = 10;
         rg1.state = 1;
         inputWait = 20; 
         rt.AddGap(rg1, inputWait);
         
         TimeGap rg2;
         rg2.duration = 0;
         rg2.state = 1;
         inputWait = 30;
         rt.AddGap(rg2, inputWait);
         
         TimeGap rg3;
         rg3.duration = 5;
         rg3.state = 0;
         inputWait = 2;
         rt.AddGap(rg3, inputWait);
         
         rt.OptimizeRelay();
         rt.PrintList();
         i = 2;*/
    }
}
uint8_t* stackptr;
uint8_t* heapptr;

long getFreeMemory() 
{
    stackptr = (uint8_t *) malloc(4);
    heapptr  = stackptr;
    free(stackptr);
    stackptr = (uint8_t *) (SP);
    Serial.print(long (heapptr));
    Serial.print("<-Heap Stack ->");
    Serial.println(long (stackptr));
    return long(stackptr) - long(heapptr);
}

