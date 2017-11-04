#pragma once
#include "SamusStateStart.h"
#include "SamusStateRunning.h"
#include "SamusStateStanding.h"
#include "SamusStateRolling.h"
#include "SamusStateJumping.h"

class SamusStateManager
{
private:
	static SamusStateManager* instance;

	SamusStateStart* stateStart;
	SamusStateRunning* stateRunning;
	SamusStateStanding* stateStanding;
	SamusStateRolling* stateRolling;
	SamusStateJumping* stateJumping;

	BaseState* currentState;

public:
	static SamusStateManager* getInstance();

	SamusStateManager();
	~SamusStateManager();
	void init(Samus* samus, Input* input);

	BaseState* getCurrentState();

	void changeStateTo(eStatus eStatus);
};
