#include "Bullet.h"
#include "BulletPool.h"
#include "Collision.h"
#include "GateBlue.h"
#include "MaruMari.h"
#include "GameLog.h"
#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Zeb.h"
#include "Rio.h"
#include "BossKraid.h"
#include "MotherBrain.h"
#include "AlienBig.h"
#include "AlienSmall.h"
#include "Ripper.h"
#include "Sound.h"
#include "Brick.h"
#include "DefendBoss.h"
#include "Buble.h"
#define WIDTH_BULLET_HALF 1
#define HEIGHT_BULLET_HALF 1

Bullet::Bullet(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BULLET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	this->indexSprite = IndexManager::getInstance()->samusYellowBulletNormal;
	this->indexEffect = IndexManager::getInstance()->samusYellowBulletNormalColliding;
	// Set Data for sprite
	this->sprite->setData(indexSprite);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->distance = 0;
	this->isCollided = false;
	this->timer = 0;
	this->listCollide = new list<CollisionReturn>();

	this->dame = 1; // se setup lai sau

	this->distanceShoot = DISTANCE_SHOOT;
}

Bullet::Bullet()
{

}


Bullet::~Bullet()
{
	delete this->sprite;
	delete this->listCollide;
}

void Bullet::onCollision()
{
	for (auto i = this->listCollide->begin(); i != this->listCollide->end(); i++)
	{
		if (i->object->isActivitied())
		{
			switch (i->object->getId())
			{
				case eID::BRICK:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);
					Brick* brick = static_cast<Brick*>(i->object);
					if(brick->isActivitied()&&brick->getVisible())
					{
						this->isCollided = true;

						this->velocity = VECTOR2ZERO;

						this->sprite->setData(indexEffect);
						
						
						brick->decreaseHealth(dame);
						switch (i->direction)
						{
						case CollideDirection::BOTTOM: case CollideDirection::TOP:
							this->setPositionY(i->positionCollision);
							break;

						case CollideDirection::LEFT: case CollideDirection::RIGHT:
							this->setPositionX(i->positionCollision);
							break;
						default:
							break;
						}

					}
					
				}

				case eID::WALL :
				case eID::GATERED: 
				case eID::DEFENSEBOSS:
				{
					Sound::getInstance()->stop(SOUND_ENEMIES_COLLISION);
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					this->sprite->setData(indexEffect);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					break;
				}
					
				
				case eID::GATEBLUE:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					this->sprite->setData(indexEffect);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;


					GateBlue* gate = static_cast<GateBlue*>(i->object);
					gate->setHit(true);

					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					break;
			}
				
				case eID::FIRE_BUBLE:
				{


					Buble* skree = static_cast<Buble*>((*i).object);

					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					this->isCollided = true;

					this->velocity = VECTOR2ZERO;

					this->sprite->setData(indexEffect);

					skree->setBeHit(true);
					skree->decreaseHealth(this->dame);

					

					break;
				}
				case eID::SKREE:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);
				
					Skree* skree = static_cast<Skree*>((*i).object);
					if (!skree->getHandle())
					{
						break;
					}
					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					this->isCollided = true;

					this->velocity = VECTOR2ZERO;

					this->sprite->setData(indexEffect);
					if (!skree->getCold())
					{
						skree->setBeHit(true);
						skree->decreaseHealth(this->dame);
						
					}
					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (skree->getCold())
						{
							skree->setCold(false);
							
						}
						else
						{
							skree->setCold(true);
						}
					}

					break;
				}
				case eID::RIO:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					Rio* rio = static_cast<Rio*>((*i).object);
					if (!rio->getHandle())
					{
						break;
					}
					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					this->sprite->setData(indexEffect);
					if (!rio->getCold())
					{
						rio->setBeHit(true);
						rio->decreaseHealth(this->dame);

					}
					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (rio->getCold())
						{
							rio->setCold(false);

						}
						else
						{
							rio->setCold(true);
						}
					}

					break;
				}
				case eID::RIPPER:
				{
					Sound::getInstance()->play(SOUND_RIPPER, false);

					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					this->sprite->setData(indexEffect);
					Ripper* ripper = static_cast<Ripper*>((*i).object);

					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (ripper->getCold())
						{
							ripper->setCold(false);
							ripper->reInit();
						}
						else
						{
							ripper->setCold(true);
						}
					}

					break;
				}
				case eID::ZOMMER:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					
					Zommer* zommer = static_cast<Zommer*>((*i).object);
					if(!zommer->getHandle())
					{
						break;
					}
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					if (!zommer->getCold())
					{
						zommer->setBeHit(true);
						zommer->decreaseHealth(this->dame);
						this->sprite->setData(indexEffect);
					}
					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (zommer->getCold())
						{
							zommer->setCold(false);
							this->sprite->setData(indexEffect);
						}
						else
						{
							zommer->setCold(true);
						}
					}
					
					break;
				}
				case eID::WAVER:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					
					Waver* waver = static_cast<Waver*>(i->object);
					if (!waver->getHandle())
					{
						break;
					}
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					if (!waver->getCold())
					{
						waver->setBeHit(true);
						waver->decreaseHealth(this->dame);
						this->sprite->setData(indexEffect);
					}
					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (waver->getCold())
						{
							waver->setCold(false);
							this->sprite->setData(indexEffect);
						}
						else
						{
							waver->setCold(true);
						}
						
					}
					
					break;
				}
				case eID::ZEB:
				{	
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					
					Zeb* zeb = static_cast<Zeb*>(i->object);
					if (!zeb->getHandle())
					{
						break;
					}
					this->velocity = VECTOR2ZERO;
					this->isCollided = true;
					switch (i->direction)
					{
					case CollideDirection::BOTTOM: case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;

					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					default:
						break;
					}

					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (zeb->getCold())
						{
							zeb->setCold(false);
							this->sprite->setData(indexEffect);
						}
						else
						{
							zeb->setCold(true);
						}
					}
					zeb->setBeHit(true);
					zeb->decreaseHealth(this->dame);
					
					break;
				}
				case eID::BOSSKRAID:
				{
					Sound::getInstance()->stop(SOUND_ENEMIES_COLLISION);
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					BossKraid* bossKraid = static_cast<BossKraid*>(i->object);
					if (bossKraid->isActivitied()) 
					{
						switch (i->direction)
						{
						case CollideDirection::BOTTOM: case CollideDirection::TOP:
							this->setPositionY(i->positionCollision);
							break;

						case CollideDirection::LEFT: case CollideDirection::RIGHT:
							this->setPositionX(i->positionCollision);
							break;
						default:
							break;
						}

						this->isCollided = true;
						this->velocity = VECTOR2ZERO;
						this->sprite->setData(indexEffect);


						bossKraid->setBeHit(true);
						bossKraid->decreaseHealth(this->dame);
					}
					break;
				}
				case eID::MOTHERBRAIN:
				{		
					Sound::getInstance()->stop(SOUND_ENEMIES_COLLISION);
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					MotherBrain* motherBrain = static_cast<MotherBrain*>(i->object);
					if (motherBrain->isActivitied())
					{
						switch (i->direction)
						{
						case CollideDirection::BOTTOM: case CollideDirection::TOP:
							this->setPositionY(i->positionCollision);
							break;

						case CollideDirection::LEFT: case CollideDirection::RIGHT:
							this->setPositionX(i->positionCollision);
							break;
						default:
							break;
						}

						this->isCollided = true;
						this->sprite->setData(indexEffect);
						this->velocity = VECTOR2ZERO;
						motherBrain->setBeHit(true);
						motherBrain->decreaseHealth(this->dame);
					}
					break;
				}
				case eID::ALIENBIG:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					this->sprite->setData(indexEffect);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					AlienBig* alienBig = static_cast<AlienBig*>(i->object);
					alienBig->setBeHit();
					break;
				}
				case eID::ALIENSMALL:
				{
					Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);

					this->sprite->setData(indexEffect);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					AlienSmall* alienSmall = static_cast<AlienSmall*>(i->object);
					alienSmall->setBeHit();
					break;
				}
				default:
					break;
			}
		}
	}

	this->listCollide->clear();
}


void Bullet::handleVelocity(float dt)
{

	if (this->distance < this->distanceShoot)
	{
		this->distance += VELOCITY_BULLET*dt;

		setBoundCollision();
	}
	else
	{
		BulletPool::getInstance()->returnPool(this);
	}

}

void Bullet::update(float dt)
{
	if(this->isCollided)
	{
		timer += dt;
		if (timer > 0.1)
		{
			BulletPool::getInstance()->returnPool(this);
		}
	}
	this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
}

void Bullet::draw()
{
	this->sprite->draw();
}

void Bullet::setBoundCollision()
{
	boundCollision.left = getPosition().x - WIDTH_BULLET_HALF;
	boundCollision.right = getPosition().x + WIDTH_BULLET_HALF;
	boundCollision.top = getPosition().y + HEIGHT_BULLET_HALF;
	boundCollision.bottom = getPosition().y - HEIGHT_BULLET_HALF;
}

void Bullet::init(VECTOR2 stPosition)
{
	if (BulletPool::getInstance()->getCurrentIceBullet())
	{
		Sound::getInstance()->stop(SOUND_BULLET_ICE);
		Sound::getInstance()->play(SOUND_BULLET_ICE, false);
	}
	else
	{
		Sound::getInstance()->stop(SOUND_BULLET_NORMAL);
		Sound::getInstance()->play(SOUND_BULLET_NORMAL, false);
	}


	this->setPosition(stPosition);
	setBoundCollision();

	this->distance = 0;
	this->setStatus(eStatus::RUNNING);
	this->isActivity = true;
}

void Bullet::returnPool()
{
	this->setStatus(eStatus::ENDING);
	this->setPosition(VECTOR2ZERO);
	setBoundCollision();
	this->sprite->setData(indexSprite);
	isCollided = false;
	this->timer = 0;
	this->isActivity = false;
	this->velocity = VECTOR2ZERO;
}

list<CollisionReturn>* Bullet::getListCollide()
{
	return this->listCollide;
}

float Bullet::getDistanceShoot()
{
	return this->distanceShoot;
}

void Bullet::setDistanceShoot(float distan)
{
	this->distanceShoot = distan;
}

void Bullet::setIceBullet()
{
	this->indexSprite = IndexManager::getInstance()->samusPinkBulletIce;
	this->indexEffect = IndexManager::getInstance()->samusPinkBulletIceColliding;
	this->sprite->setData(indexSprite);
}

