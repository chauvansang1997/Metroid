
#include "SamusStateInjuring.h"
#include "SamusStateManager.h"

#define TIME_ANIMATION 0.1f
#define TIME_TO_NORMAL 0.5f
SamusStateInjuring::SamusStateInjuring()
{
}
void SamusStateInjuring::setBoundCollision()
{
	SamusStateManager::getInstance()->getOldState()->setBoundCollision();
}
SamusStateInjuring::SamusStateInjuring(Samus * samus, Input * input) : BaseState(samus, input)
{
	
}

SamusStateInjuring::~SamusStateInjuring()
{

}

void SamusStateInjuring::init()
{

	//this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);

	
	setBoundCollision();

	time_to_stand = 0;
	time_animation = 0;

	this->samus->setHealth(this->samus->getHealth() - 8);
}

void SamusStateInjuring::handleInput(float dt)
{
	this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);
	this->samus->setVelocityX(-SAMUS_VERLOCITY_X*this->samus->getDirection());
}

void SamusStateInjuring::onCollision(float dt)
{
	MetroidRect bound;
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:
		case eID::BRICK:
		case eID::FIRE:
		case eID::ELEVATOR:
		case eID::PORT:
		case eID::GATEBLUE:
		case eID::GATERED:

			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				if (SamusStateManager::getInstance()->getOldStatus() == eStatus::RUNNING ||
					SamusStateManager::getInstance()->getOldStatus() == eStatus::STANDING)
				{
					this->samus->setPositionY(i->positionCollision + OFFSET_STAND);
					//this->samus->setStatus(eStatus::STANDING);
				}
				else if(SamusStateManager::getInstance()->getOldStatus() == eStatus::JUMPING)
				{
					this->samus->setPositionY(i->positionCollision + OFFSET_STAND);
				}
				if(SamusStateManager::getInstance()->getOldStatus() == eStatus::ROLLING || 
					SamusStateManager::getInstance()->getOldStatus() == eStatus::FALLING_ROLLING)
				{
					this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
				}
				break;
			case CollideDirection::LEFT:
				if (SamusStateManager::getInstance()->getOldStatus() == eStatus::RUNNING ||
					SamusStateManager::getInstance()->getOldStatus() == eStatus::ROLLING|| 
					SamusStateManager::getInstance()->getOldStatus() == eStatus::STANDING)
				{
					
					if (!(this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top))
					{
						return;
					}
				}
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::RIGHT:
				if (SamusStateManager::getInstance()->getOldStatus() == eStatus::RUNNING ||
					SamusStateManager::getInstance()->getOldStatus() == eStatus::ROLLING ||
					SamusStateManager::getInstance()->getOldStatus() == eStatus::STANDING)
				{
					
					if (!(this->samus->getBoundCollision().bottom< i->object->getBoundCollision().top))
					{
						return;
					}
				}
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			break;

		case eID::ZOMMER:
		case eID::SKREE:
		case eID::RIO:
		case eID::RIPPER:
		case eID::WAVER:
		case eID::ZEB:
		case eID::BOSSKRAID:
			switch (i->direction)
			{
			case CollideDirection::TOP:
				
				this->samus->setVelocityY(0);

				break;
			case CollideDirection::LEFT:
				if (SamusStateManager::getInstance()->getOldStatus() == eStatus::RUNNING || SamusStateManager::getInstance()->getOldStatus() == eStatus::ROLLING)
				{
					bound = Collision::getInstance()->getSweptBroadphaseRect(this->samus->getBoundCollision(), VECTOR2(this->samus->getVelocity().x, 0), dt);
					if (!Collision::getInstance()->isCollide(bound, i->object->getBoundCollision()))
					{
						return;
					}
				}
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::RIGHT:
				if (SamusStateManager::getInstance()->getOldStatus() == eStatus::RUNNING || SamusStateManager::getInstance()->getOldStatus() == eStatus::ROLLING)
				{
					bound = Collision::getInstance()->getSweptBroadphaseRect(this->samus->getBoundCollision(), VECTOR2(this->samus->getVelocity().x, 0), dt);
					if (!Collision::getInstance()->isCollide(bound, i->object->getBoundCollision()))
					{
						return;
					}
				}
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			//this->samus->setVelocity(VECTOR2(0, 0));
			break;
		default:
			break;
		}
	}
}

void SamusStateInjuring::update(float dt)
{
	time_to_stand += dt;
	time_animation += dt;

	this->samus->updateVertical(dt);
	this->samus->updateHorizontal(dt);
	setBoundCollision();
	if(time_animation >= TIME_ANIMATION)
	{
		if(this->samus->getVisible())
		{
			this->samus->setVisible(false);
		}
		else
		{
			this->samus->setVisible(true);
		}
		time_animation = time_animation - TIME_ANIMATION;
	}
	if(time_to_stand >= TIME_TO_NORMAL)
	{
		//if(this->)
		this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
	}

	if (this->samus->getStatus() != eStatus::INJURING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::ROLLING:
			this->samus->setVelocityX(0);
			break;
		default:
			break;
		}
		this->samus->setVisible(true);
		this->samus->setCanMoveLeft(true);
		this->samus->setCanMoveRight(true);
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
}

void SamusStateInjuring::onStart()
{
}

void SamusStateInjuring::onExit()
{
}

void SamusStateInjuring::setOldStatus(eStatus status)
{
	this->oldStatus = status;
}

eStatus SamusStateInjuring::getOldStatus()
{
	return this->oldStatus;
}


