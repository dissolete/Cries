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
	for(std::list<Aspect*>::const_iterator ai = aspects.begin(); ai != aspects.end(); ++ai){
		(*ai)->Tick(dt);
	}
}

void Entity381::SetStatus(Status newStatus)
{
	//If becoming alerted, begin to follow the player
	if(newStatus == Status::ALERTED)
	{
		aiAsp->SetCommand(new Pursue(this, engine->gfxMgr->cameraNode));
	} else if(newStatus == Status::SEARCHING)
	{
		//^If the monster can't find the player anymore, search last position
		aiAsp->SetCommand(new Search(this, engine->gfxMgr->cameraNode->getPosition()));
	} else //Must be waiting now, do nothing
	{
		aiAsp->clear();
		//Stop moving
		desiredSpeed = 0;

	}

	theStatus = newStatus;
}

HearNo::HearNo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::HEARNO, pos, heading, eng){
	this->meshfile = "See-No.mesh";
	this->acceleration = 1.0f;
	this->turnRate = 0.1f;
	this->maxSpeed = 35;
}

HearNo::~HearNo(){

}

void HearNo::Tick(float dt)
{
	//For now, no matter what this guy sees you and begins following
	if(theStatus != Status::ALERTED)
	{
		SetStatus(Status::ALERTED);
	}

	Entity381::Tick(dt);
}

SeeNo::SeeNo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::SEENO, pos, heading, eng){
	this->meshfile = "See-No.mesh";
	this->acceleration = 1.0f;
	this->turnRate = 0.1f;
	this->maxSpeed = 35;
}

SeeNo::~SeeNo()
{

}

void SeeNo::Tick(float dt)
{
	if(engine->inputMgr->isSprinting)
	{
		SetStatus(Status::ALERTED);
	} else
	{
		//If the monster can't hear the player anymore
		if(theStatus == Status::ALERTED)
		{
			SetStatus(Status::SEARCHING);
		}
	}

	Entity381::Tick(dt);
}

SpeakNo::SpeakNo(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::SPEAKNO, pos, heading, eng){
	this->meshfile = "See-No.mesh";
	this->acceleration = 1.0f;
	this->turnRate = 0.1f;
	this->maxSpeed = 35;
}

SpeakNo::~SpeakNo()
{

}

void SpeakNo::Tick(float dt)
{
	//For now, no matter what this guy sees you and begins following
	if(theStatus != Status::ALERTED)
	{
		SetStatus(Status::ALERTED);
	}

	Entity381::Tick(dt);
}

Wall::Wall(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::WALL, pos, heading, eng){
	this->meshfile = "cube.mesh";
}

Wall::~Wall()
{

}

void Wall::Tick(float dt)
{
	Entity381::Tick(dt);
}

Arch::Arch(Ogre::Vector3 pos, float heading, Engine *eng) : Entity381(EntityType::ARCH, pos, heading, eng){
	this->meshfile = "arch.mesh";
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




