#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "SkreeEffectDeath.h"
#include "Samus.h"


class Skree : public BaseObject
{
private:
	Animation *animationRotate;
	SkreeEffectDeath *effectDeath;
	Animation *explosion;

	VECTOR2 target;

	float timerDeath;

	// support to reinit
	VECTOR2 initPosition;
	bool isActivity;
	bool canDraw;


	bool beHit;
	float timerHit;
	float health;

public:
	Skree(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Skree();
	~Skree();

	bool checkCollision(Samus* sam, float dt);
	void onCollision(Samus* sam); // check collision skree's bullets with samus

	void update(float dt);
	void draw();
	void release();

	VECTOR2 getTarget();
	void setTarget(VECTOR2 target);

	void setBoundCollision();

	// set init
	void setInitPosition(VECTOR2 pos);

	void reInit();

	void finish();

	void setActivity(bool flag);
	bool isActivitied();

	void setBeHit(bool flag);
	void decreaseHealth(float dame);
};

