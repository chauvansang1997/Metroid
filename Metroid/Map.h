#pragma once
#include "constants.h"
#include "MapInfo.h"
#include "textureManager.h"
#include "graphics.h"
#include "Camera.h"

class Map
{
private:
	MapInfo* info;
	TextureManager* texture;
	Graphics* graphics;

	Camera *camera;

	bool isContain(RECT rect1, RECT rect2);

public:
	Map();
	~Map();

	bool initialize(Graphics* graphics, TextureManager* texture, MapInfo* info);
	void setCamera(Camera* camera);

	void draw();
	void update(float dt);
	void collision();
};

