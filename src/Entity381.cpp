/*
 * Entity381.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#include <Entity381.h>
#include <Aspect.h>
#include <UnitAI.h>
#include <Types.h>
#include <cmath>
#include <cstdlib>

unsigned int Entity381::nextId = 0;

Entity381::Entity381(EntityType entType, Ogre::Vector3 pos, float heading, Engine *eng){
	entityType = entType;
	this->pos = pos;
	this->heading = heading;
	this->vel = Ogre::Vector3::ZERO;
	this->speed = 0;

	this->aspects.clear();
	Renderable *r = new Renderable(this);
	Physics *p = new Physics(this);
	aiAsp = new UnitAI(this);

	this->aspects.push_front(r);
	this->aspects.push_front(p);
	this->aspects.push_front(aiAsp);

	this->entityId = Entity381::nextId++;

	DefaultInit();

	engine = eng;

	theStatus = Status::WAITING;
}

Entity381::~Entity381(){

}

void Entity381::DefaultInit(){
	this->acceleration = 0.5f;
	this->turnRate = 0.2f;

	this->maxSpeed = 100;
	this->minSpeed = 0;

	this->desiredHeading = 0;
	this->desiredSpeed = 0;

	this->meshfile = "ogrehead.mesh";

}

void Entity381::Tick(float dt){
	if(engine->theState == STATE::GAMEPLAY)
	{
		for(std::list<Aspect*>::const_iterator ai = aspects.begin(); ai != aspects.end(); ++ai){
			(*ai)->Tick(dt);
		}
	}
}

void Entity381::SetStatus(Status newStatus)
{
	//If becoming alerted, begin to follow the player
	if(newStatus == Status::ALERTED)
	{
		if(entityType == EntityType::HEARNO || entityType == EntityType::SPEAKNO)
		{
			aiAsp->SetCommand(new Pursue(this, engine->gfxMgr->cameraNode));
		} else //Must be seeno
		{
			aiAsp->SetCommand(new PursuePath(this, engine->gfxMgr->cameraNode));
		}

	} else if(newStatus == Status::SEARCHING)
	{
		//^If the monster can't find the player anymore, search last position
		if(entityType == EntityType::HEARNO)
		{
			aiAsp->SetCommand(new Search(this));
		} else //Search via hearing
		{
			aiAsp->SetCommand(new SearchPath(this, engine->gfxMgr->cameraNode->getPosition()));
		}
	} else //Must be waiting now, do nothing
	{
		aiAsp->clear();
		//Stop moving
		desiredSpeed = 0;

	}

	theStatus = newStatus;
}

float Entity381::myAngleBetween(Ogre::Vector3 v1, Ogre::Vector3 v2)
{
	float result = std::abs(Ogre::Math::ACos(v1.dotProduct(v2) / (v1.length() * v2.length())).valueDegrees());
	return result;
}

bool Entity381::canSee(Ogre::Vector3 player)
{

	DIRECTION lookDir;
	GridParams *location = engine->gameMgr->grid->getPos(ogreSceneNode->getPosition());
	GridParams *playerLocation = engine->gameMgr->grid->getPos(player);

	//Assumes a heading of 0 is East
	//0.393 is 22.5 degrees in radians

	//std::cout << "Heading: " << heading << std::endl;
	if((heading >= 0.0 && heading < 0.393) || (heading <= 0.0 && heading > -0.393))
	{
		lookDir = DIRECTION::EAST;

		//std::cout << "East" << std::endl;
		//1.178 = 67.5 degrees
	} else if(heading >= 0.393 && heading < 1.178)
	{
		lookDir = DIRECTION::SOUTHEAST;
		//std::cout << "SouthEast" << std::endl;
		//1.963= 112.5 degrees
	} else if(heading >= 1.178 && heading < 1.963)
	{
		lookDir = DIRECTION::SOUTH;
		//std::cout << "South" << std::endl;
		//2.749 = 157.5 degrees
	} else if(heading >= 1.963 && heading < 2.749)
	{
		lookDir = DIRECTION::SOUTHWEST;
		//std::cout << "Southwest" << std::endl;
	} else if((heading >= 2.749 && heading <= 3.14159) || (heading >= -3.14159 && heading <= -2.749))
	{
		lookDir = DIRECTION::WEST;
		//std::cout << "West" << std::endl;
	} else if (heading <= -0.393 && heading > -1.178)
	{
		lookDir = DIRECTION::NORTHEAST;
		//std::cout << "NorthEast" << std::endl;
	} else if (heading <= -1.178 && heading > -1.963)
	{
		lookDir = DIRECTION::NORTH;
		//std::cout << "North" << std::endl;
	} else
	{
		//Must be northwest
		lookDir = DIRECTION::NORTHWEST;
		//std::cout << "northwest, " << heading << std::endl;
	}

	//Now search in lookDir for the player
	while(location != NULL)
	{
		if(location == playerLocation)
		{
			return true;
		} else if(! location->isWalkable())
		{
			//If its a wall, return false. wall between statue and player
			return false;
		}
		//Get next location
		switch(lookDir)
		{
		case DIRECTION::EAST :
			location = engine->gameMgr->grid->getEast(location);
			break;
		case DIRECTION::NORTH :
			location = engine->gameMgr->grid->getNorth(location);
			break;
		case DIRECTION::NORTHEAST :
			location = engine->gameMgr->grid->getNE(location);
			break;
		case DIRECTION::NORTHWEST :
			location = engine->gameMgr->grid->getNW(location);
			break;
		case DIRECTION::SOUTH :
			location = engine->gameMgr->grid->getSouth(location);
			break;
		case DIRECTION::SOUTHEAST :
			location = engine->gameMgr->grid->getSE(location);
			break;
		case DIRECTION::SOUTHWEST :
			location = engine->gameMgr->grid->getSW(location);
			break;
		default :
			//Must be west
			location = engine->gameMgr->grid->getWest(location);
			break;
		}
	}

	return false;
}

bool Entity381::collides(Entity381 *other)
{
	Ogre::Vector3 diff = (other->pos - pos);

	if(other->collShape == COLLISION_SHAPE::SQUARE && collShape == COLLISION_SHAPE::SQUARE)
	{
		return (diff.x < other->collisionRange + collisionRange) || (diff.y < other->collisionRange + collisionRange);
	} else if(other->collShape == COLLISION_SHAPE::SQUARE && collShape == COLLISION_SHAPE::CIRCULAR)
	{
		diff *= -1;//Need to flip directions]
		setMagnitude(diff, collisionRange);
		Ogre::Vector3 collisionPt = pos + diff;
		diff = other->pos - collisionPt;
		return (diff.x < other->collisionRange) || (diff.y < other->collisionRange);

	} else if(other->collShape == COLLISION_SHAPE::CIRCULAR && collShape == COLLISION_SHAPE::SQUARE)
	{
		//Use the previous if statement's code
		return other->collides(this);
	} else //Must be circle and circle
	{
		return diff.length() < other->collisionRange + collisionRange;
	}
}

HearNo::HearNo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::HEARNO, pos, heading, eng){
	this->meshfile = "See-No.mesh";
	this->acceleration = 5.0f;
	this->turnRate = 1.0f;
	this->maxSpeed = 70;

	collShape = COLLISION_SHAPE::CIRCULAR;
	collisionRange = 50.0f;

	// Sounds for this entity
	eng->soundMgr->createSource("HearNoSource");
	eng->soundMgr->loadAudio("HearNoScream1", "resources/Cries - Monster Scream 1.wav");
	eng->soundMgr->loadAudio("HearNoScream2", "resources/Cries - Monster Whine.wav");

}

HearNo::~HearNo(){

}

void HearNo::Tick(float dt)
{
	this->engine->soundMgr->setSourceLocation("HearNoSource", this->pos);

	//If the enemy isn't alerted, check to see if it can see the player
	if(theStatus == Status::WAITING || theStatus == Status::SEARCHING)
	{
		if(canSee(engine->gfxMgr->cameraNode->getPosition()))
		{
			SetStatus(Status::ALERTED);

			// Randomly select one of the alerted sounds
			int randNum = rand() % 2;
			if(randNum == 1)
			{
				if(!engine->soundMgr->isSourcePlaying("HearNoSource"))
				{
					engine->soundMgr->playAudio("HearNoScream1", "HearNoSource");
				}
			}
			else
			{
				if(!engine->soundMgr->isSourcePlaying("HearNoSource"))
				{
					engine->soundMgr->playAudio("HearNoScream2", "HearNoSource");
				}
			}
		}
	} else if(theStatus == Status::ALERTED)
	{
		//If he can no longer see the player, search where he last saw the player
		if(!canSee(engine->gfxMgr->cameraNode->getPosition()))
		{
			SetStatus(Status::SEARCHING);
		}
	}

	Entity381::Tick(dt);
}

SeeNo::SeeNo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::SEENO, pos, heading, eng){
	this->meshfile = "See-No.mesh";
	this->acceleration = 5.0f;
	this->turnRate = 1.0f;
	this->maxSpeed = 70;

	collShape = COLLISION_SHAPE::CIRCULAR;
	collisionRange = 50.0f;

	// Sounds for this entity
	eng->soundMgr->createSource("SeeNoSource");
	eng->soundMgr->loadAudio("SeeNoScream1", "resources/Cries - Monster Scream 2.wav");
	eng->soundMgr->loadAudio("SeeNoScream2", "resources/Cries - Monster Scream 4.wav");
}

SeeNo::~SeeNo()
{

}

void SeeNo::Tick(float dt)
{
	if(theStatus == Status::WAITING || theStatus == Status::SEARCHING)
	{
		if(engine->inputMgr->isSprinting && (pos - engine->gfxMgr->cameraNode->getPosition()).length() <= 600)
		{
			SetStatus(Status::ALERTED);

			// Randomly select one of the alerted sounds
			int randNum = rand() % 2;
			if(randNum == 1)
			{
				if(!engine->soundMgr->isSourcePlaying("SeeNoSource"))
				{
					engine->soundMgr->playAudio("SeeNoScream1", "SeeNoSource");
				}
			}
			else
			{
				if(!engine->soundMgr->isSourcePlaying("SeeNoSource"))
				{
					engine->soundMgr->playAudio("SeeNoScream2", "SeeNoSource");
				}
			}
		}
	} else if(theStatus == Status::ALERTED)
	{
		//If the monster can't hear the player anymore
		if(!engine->inputMgr->isSprinting)
		{
			SetStatus(Status::SEARCHING);
		}
	}


	Entity381::Tick(dt);
}

SpeakNo::SpeakNo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::SPEAKNO, pos, heading, eng){
	this->meshfile = "Speak-No.mesh";
	this->acceleration = 5.0f;
	this->turnRate = 1.0f;
	this->maxSpeed = 70;

	collShape = COLLISION_SHAPE::CIRCULAR;
	collisionRange = 50.0f;
}

SpeakNo::~SpeakNo()
{

}

void SpeakNo::Tick(float dt)
{

	if(theStatus == Status::ALERTED)
	{
		if(!canSee(engine->gfxMgr->cameraNode->getPosition()))
		{
			SetStatus(Status::SEARCHING);
		}
	} else if(theStatus == Status::SEARCHING)
	{
		if(canSee(engine->gfxMgr->cameraNode->getPosition()))
		{
			SetStatus(Status::ALERTED);
		}
	} else
	{
		//Must be waiting
		if(canSee(engine->gfxMgr->cameraNode->getPosition()))
		{
			SetStatus(Status::ALERTED);
		} else if(engine->inputMgr->isSprinting && (pos - engine->gfxMgr->cameraNode->getPosition()).length() <= 600)
		{
			SetStatus(Status::SEARCHING);
		}
	}

	Entity381::Tick(dt);
}

Wall::Wall(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::WALL, pos, heading, eng){
	this->meshfile = "Wall.mesh";
	collShape = COLLISION_SHAPE::SQUARE;
	collisionRange = 100.0f;
}

Wall::~Wall()
{

}

void Wall::Tick(float dt)
{
	Entity381::Tick(dt);
}

Arch::Arch(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::ARCH, pos, heading, eng){
	this->meshfile = "Arch2.mesh";
}

Arch::~Arch()
{
//
}

void Arch::Tick(float dt)
{
	Entity381::Tick(dt);

}

Logo::Logo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::LOGO, pos, heading, eng){
	this->meshfile = "Splash.mesh";
}

Logo::~Logo()
{
//
}

void Logo::Tick(float dt)
{
	Entity381::Tick(dt);
}




