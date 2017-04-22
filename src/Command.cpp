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


MoveTo::MoveTo(Entity381 * ent, Ogre::Vector3 location): Command(ent, COMMAND_TYPE::MOVETO){
	targetLocation = location;
	MOVE_DISTANCE_THRESHOLD = 100;
}

MoveTo::~MoveTo(){

}

void MoveTo::init(){
	Ogre::Vector3 diff = targetLocation - entity->pos;
	entity->desiredHeading  = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}

bool MoveTo::done(){
	Ogre::Vector3 diff = targetLocation - entity->pos;
	if (diff.length() < MOVE_DISTANCE_THRESHOLD){
		return true;
	} else {
		return false;
	}
}

void MoveTo::tick(float dt){
	if(done()){
		entity->desiredSpeed = 0;
	} else {
		Ogre::Vector3 diff = targetLocation - entity->pos;
		entity->desiredHeading  = atan2(diff.z, diff.x);
		entity->desiredSpeed = entity->maxSpeed;
	}
}

Follow::Follow(Entity381 *ent, Entity381* targ): Command(ent, COMMAND_TYPE::FOLLOW){
	target = targ;
	this->FOLLOW_DISTANCE = 100;
}

Follow::~Follow(){

}


Ogre::Vector3 Follow::ComputeTargetLocation(){
	float behind = target->heading + Ogre::Math::PI;
	return target->pos + Ogre::Vector3 (FOLLOW_DISTANCE * cos(behind), 0, FOLLOW_DISTANCE * sin(behind));
}

void Follow::init(){
	Ogre::Vector3 diff = ComputeTargetLocation() - entity->pos;
	entity->desiredHeading = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}

bool Follow::done(){
	return false;
}

void Follow::tick(float dt){
	//compute offset
	Ogre::Vector3 diff = ComputeTargetLocation() - entity->pos;
	entity->desiredHeading = atan2(diff.z, diff.x);
	if(diff.length() <= FOLLOW_DISTANCE){
		entity->desiredSpeed = target->speed;
	} else {
		entity->desiredSpeed = entity->maxSpeed;
	}
}



Intercept::Intercept(Entity381 * ent, Entity381 * target): Follow(ent, target) {

}

Intercept::~Intercept(){
}

Ogre::Vector3 Intercept::ComputeInterceptLocation(){

	Ogre::Vector3 diff             = target->pos - entity->pos;
	Ogre::Vector3 relativeVelocity = target->vel - entity->vel;
	Ogre::Vector3 targetLocation   = target->pos;
	float relativeSpeed = relativeVelocity.length();
	if(relativeSpeed > 0){
		float timeToIntercept = diff.length()/relativeSpeed;
		if(timeToIntercept > 0){
			targetLocation = target->pos + target->vel * timeToIntercept;
		}
	}
	return targetLocation;
}

void Intercept::runIntercept(){
	Ogre::Vector3 diff = ComputeInterceptLocation() - entity->pos;
	entity->desiredHeading = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}

void Intercept::init(void){
	runIntercept();
}

bool Intercept::done(){
	return ((entity->pos - target->pos).length() < 0.5f);
}

void Intercept::tick(float dt){
	runIntercept();
}








