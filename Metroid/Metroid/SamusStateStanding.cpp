#include "SamusStateStanding.h"
#include "SamusStateManager.h"


SamusStateStanding::SamusStateStanding()
{
}

SamusStateStanding::SamusStateStanding(Samus * samus, Input * input) : BaseState(samus, input)
{
	isPressed = false;
}

SamusStateStanding::~SamusStateStanding()
{
}

void SamusStateStanding::init()
{
	if (input->isKeyDown(VK_UP))
	{
		isPressed = true;

		// Set Data for sprite
		const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowTurnUp]);
		this->samus->getSprite()->setSpriteDataRect(data->rect);
		this->samus->getSprite()->setSpriteWidth(data->width);
		this->samus->getSprite()->setSpriteHeigth(data->height);
		this->samus->setOrigin(VECTOR2(0, 1.0f));

		return;
	}

	// Set Data for sprite
	const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowTurnRight]);
	this->samus->getSprite()->setSpriteDataRect(data->rect);
	this->samus->getSprite()->setSpriteWidth(data->width);
	this->samus->getSprite()->setSpriteHeigth(data->height);
	this->samus->setOrigin(VECTOR2(0, 1.0f));
}

void SamusStateStanding::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->setStatus(eStatus::RUNNING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
	}

	if (input->isKeyDown(VK_X))
	{
		this->samus->setStatus(eStatus::JUMPING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}

	if (input->isKeyDown(VK_UP))
	{
		isPressed = true;

		// Set Data for sprite
		const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowTurnUp]);
		this->samus->getSprite()->setSpriteDataRect(data->rect);
		this->samus->getSprite()->setSpriteWidth(data->width);
		this->samus->getSprite()->setSpriteHeigth(data->height);
		this->samus->setOrigin(VECTOR2(0, 1.0f));
	}

	if (isPressed && input->isKeyUp(VK_UP))
	{
		isPressed = false;
		
		// Set Data for sprite
		const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowTurnRight]);
		this->samus->getSprite()->setSpriteDataRect(data->rect);
		this->samus->getSprite()->setSpriteWidth(data->width);
		this->samus->getSprite()->setSpriteHeigth(data->height);
		this->samus->setOrigin(VECTOR2(0, 1.0f));
	}

	if (input->isKeyDown(VK_DOWN))
	{
		this->samus->setStatus(eStatus::ROLLING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::ROLLING);
	}

	if (input->isKeyDown(VK_Z))
	{
		//Shoot
	}
}

void SamusStateStanding::update(float dt)
{
}

void SamusStateStanding::onStart()
{
}

void SamusStateStanding::onExit()
{
}
