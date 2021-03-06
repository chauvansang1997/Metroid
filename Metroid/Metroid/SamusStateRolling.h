#pragma once
#include "BaseState.h"

class SamusStateRolling : public BaseState
{
private:
	bool move_to_fall;
	bool canStanding;

	bool flagUpDown;
	float timer;

	float timerToStartBoom;

	float offsetY;
public:
	SamusStateRolling();
	SamusStateRolling(Samus* samus, Input* input);
	~SamusStateRolling();

	void init();
	void handleInput(float dt);
	void update(float dt);
	void setBoundCollision();
	void onCollision(float dt);
	void onStart();
	void onExit();
};

