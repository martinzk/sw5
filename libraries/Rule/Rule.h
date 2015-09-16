#ifndef RULE_H
#define RULE_H



#include <Configurations.h>
#include <Condition.h>
#include <Actiont.h>

class Rule
{
	public:
		Rule();
		void AddCondition(Condition c);
		void AddAction(Action a);
		void execute();
		uint8_t numConditions, numActions;
	private:
		Action actions[MAX_ACTIONS];
		Condition conditions[MAX_CONDITIONS];
		void executeActions();
};


#endif