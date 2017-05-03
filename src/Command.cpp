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
	if (diff.length() < (entity->speed * entity->speed) / (2 * entity->acceleration)){
		entity->SetStatus(Status::WAITING);
		return true;
	} else {
		return false;
	}
}

void Search::tick(float dt){

	Ogre::Vector3 diff = targetLocation - entity->pos;
	entity->desiredHeading  = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
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

PursuePath::PursuePath(Entity381 *ent, Ogre::SceneNode *targ) : Command(ent, COMMAND_TYPE::PURSUEPATH)
{
	target = targ;
	theGrid = NULL;
}

PursuePath::~PursuePath()
{

}

void PursuePath::init()
{
	theGrid = entity->engine->gridMgr;
	path = theGrid->findPath(theGrid->getPos(entity->ogreSceneNode->getPosition()),
			theGrid->getPos(target->getPosition()));
}

void PursuePath::tick(float dt)
{
	if(!path.empty())
	{
		if(theGrid->getPos(target->getPosition()) != path.back())
		{
			//Find the new path if the player has moved
			path = theGrid->findPath(theGrid->getPos(entity->ogreSceneNode->getPosition()),
					theGrid->getPos(target->getPosition()));
		}

		//compute offset
		Ogre::Vector3 diff = theGrid->getPosition(path.front()) - entity->pos;
		entity->desiredHeading = atan2(diff.z, diff.x);
		entity->desiredSpeed = entity->maxSpeed;

		//Check to see if you have successfully finished the first move point
		if(theGrid->getPos(entity->ogreSceneNode->getPosition()) == path.front())
		{
			path.pop_front();
		}
	}
}

bool PursuePath::done()
{
	return path.empty();
}

SearchPath::SearchPath(Entity381 *ent, Ogre::Vector3 location) : Command(ent, COMMAND_TYPE::SEARCHPATH)
{
	targetLocation = location;
	MOVE_DISTANCE_THRESHOLD = 100;

	theGrid = NULL;
}

SearchPath::~SearchPath()
{

}

void SearchPath::init()
{
	theGrid = entity->engine->gridMgr;
	path = theGrid->findPath(theGrid->getPos(entity->ogreSceneNode->getPosition()),
			theGrid->getPos(targetLocation));
}

void SearchPath::tick(float dt)
{
	if(!path.empty())
	{
		//compute offset
		Ogre::Vector3 diff = theGrid->getPosition(path.front()) - entity->pos;
		entity->desiredHeading = atan2(diff.z, diff.x);
		entity->desiredSpeed = entity->maxSpeed;

		//Check to see if you have successfully finished the first move point
		if(theGrid->getPos(entity->ogreSceneNode->getPosition()) == path.front())
		{
			path.pop_front();
		}
	}
}

bool SearchPath::done()
{
	if(path.empty())
	{
		entity->SetStatus(Status::WAITING);
		return true;
	} else
	{
		return false;
	}
}







