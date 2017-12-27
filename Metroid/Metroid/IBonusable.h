#pragma once
#include "Animation.h"
#include "Sprite.h"
class IBonusable
{
protected:
	Animation *item;
	bool canDraw;
public:
	void initItem(Sprite* sprite, const int* frames, int totalFrames, float frameDelay);
	void reInit();
	void setCanDraw(bool draw);
	void start();
	void update(float dt);
	IBonusable();
	~IBonusable();
};
