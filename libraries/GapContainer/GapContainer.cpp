#include "GapContainer.h"

GapContainer::GapContainer()
{
    this->prevDurations = 0;
    this->lastTime = 0;
    
    TimeGap* h = new TimeGap();
    this->head = h;
    this->head->duration = 0;
    this->head->state = 0;
    
    TimeGap* s = new TimeGap();
    this->standard = s;
    this->standard->duration = 0;
    this->standard->state = 0;
    
    this->head->next = this->standard;
    this->standard->prev = this->head;
    this->standard->next = NULL;
    this->isPlaced = false;
}

void GapContainer::AddGap(uint32_t wait, uint32_t duration, uint8_t state)
{
    //We have to update the current duration, it has to return a value because it 
    //is used when we are calling GetState() - so t will just be discarded for now  
    uint8_t t = updateAndGetState();
    TimeGap* input = new TimeGap();
    input->duration = duration;
    input->state = state;
    this->prevDurations = 0;
    this->temp = this->head->next;
    this->temp->prev = this->head;
    this->isPlaced = false;
    this->inputWait = wait;
    insertGap(input);


    optimizeList(input);
}

uint8_t GapContainer::GetState()
{
    return updateAndGetState();
}

uint8_t GapContainer::updateAndGetState()
{
    uint32_t delta = 0; 

    this->temp = this->head->next;

    if(this->lastTime == 0)
        this->lastTime = millis();
    
    if (this->temp->next == NULL)
    {
        this->lastTime = millis();
        return this->temp->state;
    }

    //There have been an overflow in millis - kan udregnes bedre
    if(this->lastTime > millis())
    {
        this->lastTime = millis();
        delta = this->lastTime;
    }
    
    else
    {
        delta = millis() - this->lastTime;
        this->lastTime = millis();
    }

    //Check for overflow
    if(this->temp->duration - delta > this->temp->duration)
    {
        delta = delta - this->temp->duration;
        this->temp->duration = 0; 
    }
    
    else
    {
        this->temp->duration -= delta;
    }

    while(this->temp->duration == 0)
    {
        removeTemp();

        if (this->temp->next == NULL)
        {
            this->lastTime = millis();
            return this->temp->state;
        }

        if(this->temp->duration - delta > this->temp->duration)
        {
            delta = delta - this->temp->duration;
            this->temp->duration = 0; 
        }
    
        else
        {
            this->temp->duration -= delta;
        }
    }

    return this->temp->state;
}

void GapContainer::removeTemp()
{
    this->temp = this->temp->next;
    delete this->head->next;
    this->head->next = this->temp;
    this->temp->prev = this->head;
}

void GapContainer::insertGap(TimeGap* input)
{
    bool removedTemp = false;
    
    while(true)
    {
        if(input->duration < 1)
        {
            if(infiniteInput(input))
            {
                break;
            }
            
        }
        else if(this->temp->duration < 1)
        {
            infiniteTemp(input);
            break;
        }
        else
        {
            //Input starter før temp og slutter efter
            if(this->inputWait + input->duration >= this->temp->duration + this->prevDurations
               && this->inputWait <= this->prevDurations)
            {
                
                //Der skal ikke breakes - vi ved endnu ikke om input påvirker
                //næste temp gap
                removeTempAndInsertInput(input);
                removedTemp = true;
            }
            
            //Input slutter i temp, temp.duration => frem
            else if(this->inputWait <= this->prevDurations //Input starter før temp
                    && this->inputWait + input->duration > this->prevDurations //Input slutter efter temp starter
                    && this->inputWait + input->duration < this->prevDurations + this->temp->duration) //Input slutter før temp slutter
            {
                inputBeforeTempAndKeepTemp(input);
                this->temp->duration = this->prevDurations + this->temp->duration - (this->inputWait + input->duration);
                break;
            }
            
            //Input starter i temp, slutter efter. temp.duration => tilbage
            else if(this->inputWait >= this->prevDurations //Input starter efter temp starter
                    && (this->prevDurations + this->temp->duration) < this->inputWait + input->duration //Input slutter efter temp
                    && (this->prevDurations + this->temp->duration) > this->inputWait) //Input starter i temp
            {
                inputAfterTempAndKeepTemp(input);
            }
            
            //input starter og slutter i temp, så temp skal deles i to
            else if(this->inputWait + input->duration < this->temp->duration + this->prevDurations && this->inputWait > this->prevDurations)
            {
                
                insertInputAndSplitTempInTwo(input);
                break;
            }
            
            //input erstatter temp
            else if(this->inputWait == this->prevDurations &&
                    input->duration == this->temp->duration)
            {
                input->prev = this->temp->prev;
                input->next = this->temp->next;
                this->temp->prev->next = input;
                this->temp->next->prev = input; 

                //this->temp->state = input->state;
                delete this->temp;
                break;
            }
            //Der er plads imellem previous temp og ny temp, til ny tid
            else if(this->inputWait + input->duration < this->prevDurations)
            {
                inputBeforeTempAndKeepTemp(input);
                break;
            }
        }
        
        this->prevDurations += this->temp->duration;
        if(isPlaced)
        {
        	TimeGap* tempRemove = this->temp; 
            //input->prev = this->temp;
            this->temp = input->next;
            
            if(removedTemp)
            {
                delete tempRemove;
                removedTemp = false;
            }
        }
        else
        {
        	TimeGap* tempRemove = this->temp; 

            this->temp = this->temp->next;
            if(removedTemp)
            {
                delete tempRemove;
                removedTemp = false;
            }
        }
    }
}

void GapContainer::insertInputAndSplitTempInTwo(TimeGap* input)
{
    TimeGap* trg = new TimeGap();
    if(this->temp->duration == 0)
    {
        trg->duration = 0;
    }
    else
    {
        trg->duration =  this->prevDurations + this->temp->duration - (this->inputWait + input->duration);
    }
    trg->state = this->temp->state;
    trg->prev = input;
    //Check if temp is not last element
    if(trg->duration != 0)
    {
        trg->next = this->temp->next;
    }
    
    this->temp->duration = this->inputWait - this->prevDurations;
    this->temp->next = input;
    input->prev = this->temp;
    input->next = trg;
}

void GapContainer::removeTempAndInsertInput(TimeGap* input)
{
    input->next = this->temp->next;
    if(!this->isPlaced)
    {
        input->prev = this->temp->prev;
        this->temp->prev->next = input;
        this->isPlaced = true;
    }
    this->temp->next->prev = input;
}

void GapContainer::inputAfterTempAndKeepTemp(TimeGap* input)
{
    //Slutter i den følgende
    uint32_t t = this->temp->duration;
    this->temp->duration = inputWait - this->prevDurations;
    this->prevDurations += t - inputWait + this->prevDurations; //HF
    // skal lige gøres smart
    input->prev = this->temp;
    input->next = this->temp->next;
    this->temp->next->prev = input;
    this->temp->next = input;
    this->isPlaced = true;
}

void GapContainer::inputBeforeTempAndKeepTemp(TimeGap* input)
{
    if(!this->isPlaced)
    {
        input->prev = this->temp->prev;
        this->temp->prev->next = input;
        this->isPlaced = true;
    }
    input->next = this->temp;
    this->temp->prev = input;
}

void GapContainer::infiniteTemp(TimeGap* input)
{
    //skal temp splittes i to dele?
    if(this->inputWait > this->prevDurations)
    {
        insertInputAndSplitTempInTwo(input);
    }
    
    else
    {
        inputBeforeTempAndKeepTemp(input);
    }
}
void GapContainer::deleteAllAfter(TimeGap* toBefree)
{
    this->temp = toBefree->next;
    /*
    while(true)
    {
        if(this->temp->duration < 1)
        {
            delete this->temp;
            break;
        }
        else
        {
            this->temp->next->prev = this->temp;
            this->temp = this->temp->next;
           	delete this->temp->prev;
        }
    }*/
    while(this->temp->duration > 0)
    {
        this->temp->next->prev = this->temp;
        this->temp = this->temp->next;
        delete this->temp->prev;
    }

    //Temp is now the last element in the list
    delete this->temp;
}

bool GapContainer::infiniteInput(TimeGap* input)
{
    //input starter før temp
    if(this->inputWait <= this->prevDurations)
    {
        //MIS 
        TimeGap* preTemp;
        this->temp->prev->next = input;
        input->prev = this->temp->prev;
        input->next = NULL;
        preTemp = this->temp;
 
        if (this->temp->duration < 1)
        {
            delete preTemp;
            return true;
        }
       
        deleteAllAfter(this->temp);
       
        this->temp = preTemp;
        return true;
       
    }
   
    //starter input i temp?
    else if ((this->inputWait > this->prevDurations && this->inputWait < this->prevDurations + this->temp->duration) || this->temp->duration == 0)
    {
        //MIS
        TimeGap* preTemp;
        preTemp = this->temp;
       
        if(this->temp->duration >= 1)
        {
            deleteAllAfter(this->temp);
        }
 
        this->temp = preTemp;
        this->temp->duration = this->inputWait - this->prevDurations;
 
        this->temp->next = input;
        input->prev = this->temp;
        input->next = NULL;
        return true;
    }
   
    return false;
}

void GapContainer::optimizeList(TimeGap* input)
{
    this->temp = input->prev;
    TimeGap* tempGap;

    for (int i = 0; i < 2; i++)
    {
        if(this->temp->duration < 1)
        {
            break;
        }

        tempGap = this->temp->next;

        if(this->temp->state == tempGap->state)
        {
            if(tempGap->duration < 1)
            {
                this->temp->duration = 0;
                this->temp->next = NULL;
                delete tempGap; 
                break; 
            }

            else
            {
                this->temp->duration += tempGap->duration;
                this->temp->next = tempGap->next;
                tempGap->next->prev = this->temp;
                delete tempGap;
                tempGap = this->temp->next;
            }
        }

        else
        {
            this->temp = this->temp->next;
        }
    }
}