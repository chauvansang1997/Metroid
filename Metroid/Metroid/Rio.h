#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Rio: public BaseObject
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
	int flag = 0;
	//float position

	bool isSamusRolling;

	int health;

public:
	Rio(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Rio();
	~Rio();

	void update(float dt);
	void draw();

	VECTOR2 getTarget();
	void setTarget(VECTOR2 target, bool statusTarget);

	void initPositions(VECTOR2 stP);
};

