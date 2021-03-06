#pragma once
#include "BaseState.h"

class SamusStateFallRolling:public BaseState
{
private:
	float remain_time;

public:
	SamusStateFallRolling();
	SamusStateFallRolling(Samus* samus, Input* input);
	~SamusStateFallRolling();


	void init();
	void handleInput(float dt);
	void onCollision(float dt);
	void update(float dt);
	void setBoundCollision();

	void onStart();
	void onExit();
};

