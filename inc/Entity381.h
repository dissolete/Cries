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
#include <UnitAI.h>

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

public:

	Entity381(EntityType entityType, Ogre::Vector3 pos, float heading, Engine eng);
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
	Ogre::Entity* ogreEntity;

	//Engine stuff dynamic
	float speed, heading;
	float desiredSpeed, desiredHeading;

	unsigned int entityId;

	std::list<Aspect*> aspects;
	UnitAI *aiAsp;

	Engine *engine;

private:
	Status theStatus;//Stores whether or not the player has been spotted
};

class SeeNo: public Entity381 {

public:
	SeeNo(Ogre::Vector3 pos, float heading);
	~SeeNo();
	void Tick(float dt);

};

class HearNo: public Entity381 {

public:
	HearNo(Ogre::Vector3 pos, float heading);
	~HearNo();
	void Tick(float dt);
};

class SpeakNo: public Entity381 {

public:
	SpeakNo(Ogre::Vector3 pos, float heading);
	~SpeakNo();
	void Tick(float dt);
};


#endif /* ENTITY381_H_ */
