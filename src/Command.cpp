/*
 * Command.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: sushil
 */

#include <Command.h>
#include <math.h>
#include <OgreMath.h>

Command::Command(Entity381 *ent, COMMAND_TYPE ct){
	entity = ent;
	commandType = ct;
}

Command::~Command(){

}

void Command::init(){

}

void Command::tick(float dt){

}

bool Command::done(){
	return true;
}


Search::Search(Entity381 * ent, Ogre::Vector3 location): Command(ent, COMMAND_TYPE::SEARCH){
	targetLocation = location;
	MOVE_DISTANCE_THRESHOLD = 100;
}

Search::~Search(){

}

void Search::init(){
	Ogre::Vector3 diff = targetLocation - entity->pos;
	entity->desiredHeading  = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}

bool Search::done(){
	Ogre::Vector3 diff = targetLocation - entity->pos;
	if (diff.length() < MOVE_DISTANCE_THRESHOLD){
		entity->SetStatus(Status::WAITING);
		return true;
	} else {
		return false;
	}
}

void Search::tick(float dt){
	if(done()){
		entity->desiredSpeed = 0;
	} else {
		Ogre::Vector3 diff = targetLocation - entity->pos;
		entity->desiredHeading  = atan2(diff.z, diff.x);
		entity->desiredSpeed = entity->maxSpeed;
	}
}

Pursue::Pursue(Entity381 *ent, Ogre::SceneNode* targ): Command(ent, COMMAND_TYPE::PURSUE){
	target = targ;
}

Pursue::~Pursue(){

}

void Pursue::init(){
	Ogre::Vector3 diff = target->getPosition() - entity->pos;
	entity->desiredHeading = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}

bool Pursue::done(){
	return false;
}

void Pursue::tick(float dt){
	//compute offset
	Ogre::Vector3 diff = target->getPosition() - entity->pos;
	entity->desiredHeading = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}










