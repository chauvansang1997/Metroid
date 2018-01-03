#include "SamusStateBoom.h"
#include "SamusStateManager.h"
#include "BulletPool.h"
#include "GameLog.h"
#include "Camera.h"
#include "Zommer.h"
#include "Skree.h"
#include "Zeb.h"
#include "Ripper.h"
#include "Rio.h"
#include "Waver.h"
#include "Sound.h"
#include "GateBlue.h"
#include "GateRed.h"
#include "SamusStateManager.h"
#include "Port.h"
#include "ScenceManager.h"

#include "MaruMari.h"
#include "EnergyTank.h"
#include "Bomb.h"
#include "LongBeam.h"
#include "MissileRocket.h"
#include "IceBeam.h"
#include "Varia.h"
#define TIME_TO_NORMAL 0.5f

SamusStateBoom::SamusStateBoom()
{
}
void SamusStateBoom::setBoundCollision()
{
	SamusStateManager::getInstance()->getOldState()->setBoundCollision();
}
SamusStateBoom::SamusStateBoom(Samus * samus, Input * input) : BaseState(samus, input)
{
	isUp = false;
}

SamusStateBoom::~SamusStateBoom()
{
}

void SamusStateBoom::init()
{
	setBoundCollision();
	timerToRunning = 0;
	if (input->isKeyDown(VK_UP))
	{
		isUp = true;
		// Set Data for sprite
		this->samus->setDataSuiteSkin(
			IndexManager::getInstance()->samusYellowTurnUp,
			IndexManager::getInstance()->samusYellowIceTurnUp,
			IndexManager::getInstance()->samusPinkTurnUp,
			IndexManager::getInstance()->samusPinkIceTurnUp);

		return;
	}

	// Set Data for sprite
	this->samus->setDataSuiteSkin(
		IndexManager::getInstance()->samusYellowTurnRight,
		IndexManager::getInstance()->samusYellowIceTurnRight,
		IndexManager::getInstance()->samusPinkTurnRight,
		IndexManager::getInstance()->samusPinkIceTurnRight);

	canRolling = false;
}

void SamusStateBoom::handleInput(float dt)
{

	this->samus->setVelocity(VECTOR2(0, SAMUS_MIN_SPEED_Y));
	if (!Camera::getInstance()->moveWhenSamusOnPort())
	{
#pragma region Horizontal
		if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
		{
			// Handle direction
			if (this->samus->isInDirection(eDirection::left))
			{
				this->samus->setFlipX(false);
				this->samus->setDirection(eDirection::right);
			}

			this->samus->setVelocityX(SAMUS_VELOCITY_JUMP_X);
		}

		if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		{

			// Handle direction	
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setFlipX(true);
				this->samus->setDirection(eDirection::left);
			}

			this->samus->setVelocityX(-SAMUS_VELOCITY_JUMP_X);
		}
#pragma endregion
		//reset jump when user release jump button
		if (input->isKeyUp(VK_X))
		{
			this->samus->setCanJump(true);
		}

		if (input->isKeyDown(VK_X) && this->samus->canJump() == true)
		{
			this->samus->setFall(false);
			this->samus->setAcrobat(false);
			this->samus->setVelocityX(0);
			this->samus->setStatus(eStatus::JUMPING);
			return;
		}

		if (input->isKeyDown(VK_UP))
		{
			isUp = true;

			// Set Data for sprite
			this->samus->setDataSuiteSkin(
				IndexManager::getInstance()->samusYellowTurnUp,
				IndexManager::getInstance()->samusYellowIceTurnUp,
				IndexManager::getInstance()->samusPinkTurnUp,
				IndexManager::getInstance()->samusPinkIceTurnUp);
		}

		if (isUp && input->isKeyUp(VK_UP))
		{
			isUp = false;

			// Set Data for sprite
			this->samus->setDataSuiteSkin(
				IndexManager::getInstance()->samusYellowTurnRight,
				IndexManager::getInstance()->samusYellowIceTurnRight,
				IndexManager::getInstance()->samusPinkTurnRight,
				IndexManager::getInstance()->samusPinkIceTurnRight);
		}

		if (input->isKeyDown(VK_DOWN) && this->samus->isHaveMariMaru())
		{
			this->samus->setVelocityX(0);
			this->samus->setStatus(eStatus::ROLLING);
			return;
		}


#pragma region Vertical
		// Handle vertical
	}
}

void SamusStateBoom::onCollision(float dt)
{
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{

		case eID::WALL:
		case eID::BRICK:
		case eID::ELEVATOR:
		case eID::ALIENBIG:
		case eID::ALIENSMALL:
		{

			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
				this->samus->setVelocityY(0);
				break;
			case CollideDirection::LEFT:


				this->samus->setVelocityX(0);
				break;
			case CollideDirection::RIGHT:

				this->samus->setVelocityX(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
				this->samus->setVelocityY(0);
				break;
			}
			break;
		}
		case eID::FIRE:
		{
			switch (i->direction)
			{

			case CollideDirection::TOP:
				this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
				this->samus->setVelocityY(0);
				break;

			}
			break;
		}
#pragma region GATE and PORT
		case eID::GATEBLUE: case eID::GATERED:
		{
			if (i->object->isActivitied())
			{
				switch (i->direction)
				{
					case CollideDirection::LEFT:
					{
						if (Camera::getInstance()->moveWhenSamusOnPort())
						{
							GateBlue* gate = static_cast<GateBlue*>(i->object);
							gate->setIsCollideSamusInPort(true);

							this->samus->setIsCollidingPort(false);
							this->samus->setVelocityX(0);
							this->samus->setCanMoveToFrontGate(true);
						}
						else
						{
							this->samus->setVelocityX(0);
							this->samus->setCanMoveRight(false);
						}

						break;
					}
					case CollideDirection::RIGHT:
					{
						if (Camera::getInstance()->moveWhenSamusOnPort())
						{
							GateBlue* gate = static_cast<GateBlue*>(i->object);
							gate->setIsCollideSamusInPort(true);

							this->samus->setIsCollidingPort(false);
							this->samus->setVelocityX(0);
							this->samus->setCanMoveToFrontGate(true);
						}
						else
						{
							this->samus->setVelocityX(0);
							this->samus->setCanMoveLeft(false);
						}

						break;
					}
					case CollideDirection::TOP:
						this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
						this->samus->setVelocityY(0);
						break;
				}
			}

			break;
		}

		case eID::PORT:
		{
			Port* port = static_cast<Port*>(i->object);
			if (port->Win() == true)
			{
				this->samus->setVelocity(VECTOR2ZERO);
				ScenceManager::getInstance()->goToScence(ScenceType::END);
				return;
			}
			switch (i->direction)
			{

			case LEFT:

				Camera::getInstance()->setVelocity(VECTOR2(SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->samus->setIsCollidingPort(true);
				break;
			case RIGHT:
				Camera::getInstance()->setVelocity(VECTOR2(-SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->samus->setIsCollidingPort(true);
				break;
			default:
				break;
			}

			break;
		}

#pragma endregion

#pragma region enemies
		case eID::ZOMMER:
		{
			Zommer* zommer = static_cast<Zommer*>(i->object);

			if (zommer->getCold() && zommer->getHandle())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				case CollideDirection::BOTTOM:
					this->samus->setVelocityY(0);
					break;
				}
			}
			else if (zommer->getExplose())
			{
				zommer->setCanDraw(false);
				zommer->setActivity(false);
			}
			else
			{
				if (!zommer->getHandle())
				{
					return;
				}
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());

			}
			break;
		}
		case eID::WAVER:
		{
			Waver* waver = static_cast<Waver*>(i->object);
			if (waver->getCold() && waver->getHandle())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:

					this->samus->setCanMoveRight(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setCanMoveLeft(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
					break;
				case CollideDirection::BOTTOM:
					this->samus->setVelocityY(0);
					this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
					break;
				}
				break;
			}
			else if (waver->getExplose())
			{
				waver->setCanDraw(false);
				waver->setActivity(false);
			}
			else
			{
				if (!waver->getHandle())
				{
					return;
				}
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());
			}
			break;
		}
		case eID::SKREE:
		{
			Skree* skree = static_cast<Skree*>(i->object);
			if (!skree->getHandle())
			{
				return;
			}
			if (skree->getCold() && skree->getHandle())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:

					this->samus->setCanMoveRight(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setCanMoveLeft(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);

					this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
					break;
				case CollideDirection::BOTTOM:
					this->samus->setVelocityY(0);

					this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
					break;
				}
				break;
			}
			else if (skree->getExplose())
			{
				skree->setCanDraw(false);
				skree->setActivity(false);
			}
			else
			{
				if (!skree->getHandle())
				{
					return;
				}
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());
			}
			break;
		}
		case eID::RIO:
		{
			Rio* rio = static_cast<Rio*>(i->object);

			if (rio->getCold() && rio->getHandle())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:

					this->samus->setCanMoveRight(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setCanMoveLeft(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:

					this->samus->setVelocityY(0);
					break;
				case CollideDirection::BOTTOM:
					this->samus->setVelocityY(0);

					break;
				}
				break;
			}
			else if (rio->getExplose())
			{
				rio->setCanDraw(false);
				rio->setActivity(false);
			}
			else
			{
				if (!rio->getHandle())
				{
					return;
				}
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());
			}
			break;
		}
		case eID::RIPPER:
		{
			Ripper* ripper = static_cast<Ripper*>(i->object);
			if (ripper->getCold())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:

					this->samus->setCanMoveRight(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setCanMoveLeft(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				case CollideDirection::BOTTOM:
					this->samus->setVelocityY(0);
					break;
				}
				break;
			}
			else
			{
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());
			}
			break;
		}
		case eID::ZEB:
		{
			Zeb* zeb = static_cast<Zeb*>(i->object);

			if (zeb->getCold() && zeb->getHandle())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:

					this->samus->setCanMoveRight(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setCanMoveLeft(false);
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				case CollideDirection::BOTTOM:
					this->samus->setVelocityY(0);

					break;
				}
				break;
			}
			else if (zeb->getExplose())
			{
				zeb->setCanDraw(false);
				zeb->setActivity(false);
			}
			else
			{
				if (!zeb->getHandle())
				{
					return;
				}
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());
			}
			break;
		}
		case eID::BOSSKRAID:
		{
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setVelocityX(0);
				//not allow move left
				//this->samus->setCanMoveRight(false);
				break;
			case CollideDirection::RIGHT:
				this->samus->setVelocityX(0);
				//not allow move right
				//this->samus->setCanMoveLeft(false);
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
			SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());
			break;
		}
		case eID::MOTHERBRAIN:
		{
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setVelocityX(0);
				//not allow move left
				//this->samus->setCanMoveRight(false);
				break;
			case CollideDirection::RIGHT:
				this->samus->setVelocityX(0);
				//not allow move right
				//this->samus->setCanMoveLeft(false);
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				break;
			}

			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldStatus(SamusStateManager::getInstance()->getOldStatus());
			SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getOldState());

			break;
		}

#pragma endregion

#pragma region items
		case eID::MARUMARI:
		{
			this->samus->setMariMaru(true);
			MaruMari* mm = static_cast<MaruMari*>(i->object);
			mm->setActivity(false);

			break;
		}

		case eID::ENERGYTANK:
		{
			this->samus->setNumLive(this->samus->getNumLive() + 1);
			EnergyTank* energy = static_cast<EnergyTank*>(i->object);
			energy->setActivity(false);

			break;
		}

		case eID::BOMB:
		{
			this->samus->setMariMaru(true); // just test

			this->samus->setBomb(true);
			Bomb* bom = static_cast<Bomb*>(i->object);
			bom->setActivity(false);

			break;
		}

		case eID::LONGBEAM:
		{
			this->samus->setLongBeam(true);
			LongBeam* lb = static_cast<LongBeam*>(i->object);
			lb->setActivity(false);

			BulletPool::getInstance()->setDistanceShoot(DISTANCE_SHOOT * 2);

			break;
		}

		case eID::MISSILEROCKET:
		{
			this->samus->setNumRocket(this->samus->getNumRocket() + 5);
			MissileRocket* mrocket = static_cast<MissileRocket*>(i->object);
			mrocket->setActivity(false);

			break;
		}

		case eID::ICEBEAM:
		{
			IceBeam* ib = static_cast<IceBeam*>(i->object);
			ib->setActivity(false);

			if (this->samus->getCurrentSkin() == eSkin::YELLOW)
				this->samus->setNewSkin(eSkin::YELLOWICE);
			if (this->samus->getCurrentSkin() == eSkin::PINK)
				this->samus->setNewSkin(eSkin::PINKICE);

			BulletPool::getInstance()->setIceBullet();

			break;
		}

		case eID::VARIA:
		{
			Varia* va = static_cast<Varia*>(i->object);
			va->setActivity(false);

			if (this->samus->getCurrentSkin() == eSkin::YELLOW)
				this->samus->setNewSkin(eSkin::PINK);
			if (this->samus->getCurrentSkin() == eSkin::YELLOWICE)
				this->samus->setNewSkin(eSkin::PINKICE);

			break;
		}

#pragma endregion


		default:
			break;
		}
	}
}

void SamusStateBoom::update(float dt)
{

	this->samus->updateVertical(dt);
	setBoundCollision();

	if (this->samus->getStatus() != eStatus::BOOM)
	{
		//switch (this->samus->getStatus())
		//{
		//case eStatus::JUMPING:

		//	//this->samus->setPositionX(this->samus->getPosition().x )
		//	SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		//	break;
		//case eStatus::RUNNING:
		//	SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		//	break;
		//case eStatus::ROLLING:
		//	if (canRolling)
		//	{
		//		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		//	}
		//	break;
		//case eStatus::INJURING:
		//	SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		//	break;
		//default:
		//	break;
		//}

		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}

}

void SamusStateBoom::onStart()
{
}

void SamusStateBoom::onExit()
{
}

void SamusStateBoom::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();
	Sound::getInstance()->play(SOUND_BULLET, false);
	if (isUp)
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection(), this->samus->getPosition().y + this->samus->getSprite()->getHeight()*0.4f);
		bullet->setVelocity(VECTOR2(0, VELOCITY_BULLET));
	}
	else
	{
		stP = VECTOR2(this->samus->getPosition().x, this->samus->getPosition().y + 3);
		bullet->setVelocity(VECTOR2((float)VELOCITY_BULLET*this->samus->getDirection(), 0));
	}

	bullet->init(stP);
}

void SamusStateBoom::fireRocket()
{
	VECTOR2 stP;
	Rocket* rocket = RocketPool::getInstance()->getRocket();
	Sound::getInstance()->play(SOUND_ROCKET, false);
	if (isUp)
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection(), this->samus->getPosition().y + this->samus->getSprite()->getHeight()*0.4f);
		rocket->setVelocity(VECTOR2(0, VELOCITY_ROCKET));

		rocket->getSprite()->setData(IndexManager::getInstance()->rocketPinkUp);
	}
	else
	{
		stP = VECTOR2(this->samus->getPosition().x, this->samus->getPosition().y + 3);
		rocket->setVelocity(VECTOR2((float)VELOCITY_ROCKET*this->samus->getDirection(), 0));

		rocket->getSprite()->setData(IndexManager::getInstance()->rocketPinkR);

		if (this->samus->isInDirection(eDirection::left))
			rocket->getSprite()->setFlipX(true);
		else
			rocket->getSprite()->setFlipX(false);
	}

	rocket->init(stP);

	this->samus->setNumRocket(this->samus->getNumRocket() - 1);
}
