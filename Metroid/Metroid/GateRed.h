#pragma once
#include "BaseObject.h"
#include "Animation.h"

class GateRed : public BaseObject
{
public:
	GateRed();
	GateRed(TextureManager* textureM, Graphics* graphics);
	~GateRed();

	void update(float dt);
	void draw();
};

