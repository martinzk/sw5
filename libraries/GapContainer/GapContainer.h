#ifndef GAPCONTAINER_H
#define GAPCONTAINER_H
#include "Arduino.h"

typedef struct TimeGap TimeGap;
struct TimeGap
{
    uint32_t duration;
    uint8_t state;
    TimeGap* next;
    TimeGap* prev;
};

class GapContainer
{
	public:
	    GapContainer();
	    void AddGap(uint32_t wait, uint32_t duration, uint8_t state);
	    uint8_t GetState();
	private:
	    TimeGap* head;
	    TimeGap* temp;
	    TimeGap* standard;
	    uint8_t updateAndGetState();
	    void removeTemp();
	    void insertGap(TimeGap*);
	    void infiniteTemp(TimeGap*);
	    bool infiniteInput(TimeGap*);
	    void inputBeforeTempAndKeepTemp(TimeGap*);
	    void removeTempAndInsertInput(TimeGap*);
	    void insertInputAndSplitTempInTwo(TimeGap*);
	    void inputAfterTempAndKeepTemp(TimeGap*);
	    void deleteAllAfter(TimeGap*);
	    void updateTime();
	    void optimizeList(TimeGap*);
	    bool isPlaced;
	    uint32_t prevDurations, inputWait, lastTime;
};


#endif