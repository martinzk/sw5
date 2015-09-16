#include <Rule.h>

Rule::Rule()
{
    this->numConditions = 0;
    this->numActions = 0;

}

void Rule::AddCondition(Condition c)
{
    if(numConditions < MAX_CONDITIONS)
    {
        conditions[numConditions] = c;
        numConditions++; 
    }
}

void Rule::AddAction(Action a)
{
      if (numActions < MAX_ACTIONS)
      {
            actions[numActions] = a;
            numActions++;
      }
}

void Rule::execute()
{
      bool allConditionsTrue = true;
      for (int i = 0; i < numConditions; ++i)
      {
        // Ensure all conditions are true
        if(conditions[i].StateComponent->GetCurrentState() != conditions[i].GetState())
        {
          allConditionsTrue = false;
          break;
        }
      }

      if(allConditionsTrue)
      {
        this->executeActions();
      }      
}

void Rule::executeActions()
{
    for (int i = 0; i < numActions; ++i)
    {
        //Update the state component
       actions[i].StateComponent->Insert(actions[i].GetDesiredState(), actions[i].GetWait(), actions[i].GetDuration());
    }
}

