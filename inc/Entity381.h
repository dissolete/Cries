/*
 * Entity.h
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#ifndef ENTITY381_H_
#define ENTITY381_H_

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <engine.h>

#include <Types.h>


#include <list>

class Aspect;
class Engine;
class UnitAI;

class Entity381 {

private:
	//
	static unsigned int nextId;

protected:
	//
	float myAngleBetween(Ogre::Vector3 v1, Ogre::Vector3 v2);
	void setMagnitude(Ogre::Vector3 v, float mag);
	bool notObstructed(Ogre::Vector3 player, Ogre::Vector3 enemy);

public:

	Entity381(EntityType entityType, Ogre::Vector3 pos, float heading, Engine *eng);
	~Entity381();
	virtual void Tick(float dt);
	void DefaultInit();
	void SetStatus(Status newStatus);

	//static stuff
	float maxSpeed, minSpeed;
	float acceleration, turnRate;
	EntityType entityType;
	std::string meshfile;

	//Ogre stuff
	Ogre::Vector3 pos;
	Ogre::Vector3 vel;
	Ogre::SceneNode *ogreSceneNode;
	Ogre::Entity* ogreEntity, *wallEntity;

	//Engine stuff dynamic
	float speed, heading;
	float desiredSpeed, desiredHeading;

	unsigned int entityId;

	std::list<Aspect*> aspects;
	UnitAI *aiAsp;

	Engine *engine;

	Status theStatus;//Stores whether or not the player has been spotted
};

class SeeNo: public Entity381 {

public:
	SeeNo(Ogre::Vector3 pos, float heading, Engine *eng);
	~SeeNo();
	void Tick(float dt);

};

class HearNo: public Entity381 {

public:
	HearNo(Ogre::Vector3 pos, float heading, Engine *eng);
	~HearNo();
	void Tick(float dt);
};

class SpeakNo: public Entity381 {

public:
	SpeakNo(Ogre::Vector3 pos, float heading, Engine *eng);
	~SpeakNo();
	void Tick(float dt);
};

// Static Entities
class Wall: public Entity381 {
public:
	Wall(Ogre::Vector3 pos, float heading, Engine *eng);
	~Wall();
	void Tick(float dt);
};

class Arch: public Entity381 {
public:
	Arch(Ogre::Vector3 pos, float heading, Engine *eng);
	~Arch();
	void Tick(float dt);
};

class Logo: public Entity381 {
public:
	Logo(Ogre::Vector3 pos, float heading, Engine *eng);
	~Logo();
	void Tick(float dt);
};


#endif /* ENTITY381_H_ */
