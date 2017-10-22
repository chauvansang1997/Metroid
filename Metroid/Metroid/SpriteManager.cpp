#include "SpriteManager.h"

SpriteManager* SpriteManager::instance = nullptr;

const SpriteData* SpriteManager::getSpritesData()
{
	return this->spritesData;
}
SpriteManager * SpriteManager::getInstance()
{
	if (instance == nullptr)
		instance = new SpriteManager();

	return instance;
}
bool SpriteManager::initialize(const char* filename)
{
	this->filename = filename;
	try
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);
		//read spriteData from jSon
		if (jSon["frames"].IsArray())
		{

			//convert document to value to loop all item in array
			const Value& value = jSon["frames"];
			this->spritesData = new SpriteData[value.Size()];
			for (SizeType i = 0; i < value.Size(); i++)
			{
				SpriteData spriteData;
				const Value& frame = value[i]["frame"];
				//get spriteData in json



				//get is rotated in jSon, default if true sprite is rotated by  90
				spriteData.width = frame["w"].GetInt();
				spriteData.height = frame["h"].GetInt();
				spriteData.angle = 0;
				
				RECT rect;
				rect.left = frame["x"].GetInt();
				rect.top = frame["y"].GetInt();
				rect.bottom = rect.top + spriteData.height;
				rect.right = rect.left + spriteData.width;
				spriteData.rect = rect;
				this->spritesData[i] = spriteData;
			}
		}
	}
	catch (const std::exception&)
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not convert to spriteData");
	}
	return true;
}
void SpriteManager::releaseAll()
{
	delete[] spritesData;
}
SpriteManager::SpriteManager(void)
{
}


SpriteManager::~SpriteManager(void)
{
}