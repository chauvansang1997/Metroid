#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Samus.h"
#include "IFreezable.h"
class Rio: public BaseObject,public IFreezable
{
private:
	Animation *anim;

	VECTOR2 startPosition;
	VECTOR2 target;
	VECTOR2 P1;// P1 in B�zier curve
	VECTOR2 P2;// P2 in B�zier curve
	VECTOR2 P3;// P3 in B�zier curve
	VECTOR2 P4;// P4 in B�zier curve
	VECTOR2 P5;// P1 in B�zier curve


	// Use B�zier curve
	float t;
	float t1;
	int flag = 1;
	bool start = false;
	float positionBeforeX;
	float positionAfterX;

	float positionBeforeY;
	float positionAfterY;

	int health;

public:
	Rio(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Rio();
	~Rio();

	void update(float dt);
	void draw();
	void setBoundCollision();

	VECTOR2 getTarget();
	void setTarget(VECTOR2 target);

	void initPositions(VECTOR2 stP);
};

