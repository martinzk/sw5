#include <StateContainer.h>

bool StateContainer::instanceFlag = false;
// singleton
StateContainer* StateContainer::sc = NULL;

StateContainer* StateContainer::GetStateContainer()
{
	if(!instanceFlag)
	{
		sc = new StateContainer();
		instanceFlag = true;
	}
	return sc; 
}

StateContainer::StateContainer()
{
	this->index = 0;
	this->modeindex = 0;
	this->alarmIndex = 0;
}

uint8_t StateContainer::GetLength()
{
	return this->index;
}

void StateContainer::AddAlarm(Alarm a)
{
	alarms[alarmIndex] = a;
	alarmIndex++;
	this->AddStateComponent(&a);
}

void StateContainer::AddMode(Mode m)
{
	modes[modeindex] = m;
	modeindex++;
	this->AddStateComponent(&m);
}

void StateContainer::AddStateComponent(IStateComponent *is)
{
	if(index < UINT8MAX)
	{
		this->stateComponents[index] = is;
		index++;
	}
}

void StateContainer::ListenForStateComponents()
{
	for (uint8_t i = 0; i < index; ++i)
	{   
		stateComponents[i]->SetState();
	}
}



IStateComponent* StateContainer::GetStateComponent(uint8_t id)
{
	for (int i = 0; i < index; ++i)
	{
		if(this->stateComponents[i]->GetID() == id)
		{
			return stateComponents[i];
		}
	}
}
