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
		entity->desiredHeading = FixAngle(entity->heading + Ogre::Math::PI);
		entity->speed = 0;
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
	std::cout << "PursuePath made" << std::endl;
	target = targ;
	theGrid = NULL;
	init();
}

PursuePath::~PursuePath()
{

}

void PursuePath::init()
{
	theGrid = entity->engine->gameMgr->grid;
	path = theGrid->findPath(theGrid->getPos(entity->ogreSceneNode->getPosition()),
			theGrid->getPos(target->getPosition()));
	std::cout << "Path: " << &path << std::endl;

	std::cout << "Path length: " << path.size() << std::endl;
}

void PursuePath::tick(float dt)
{
	std::cout << "Made it inside PursuePath" << std::endl;
	if(!path.empty() && path.front() != NULL && path.back() != NULL)
	{
		std::cout << "Made it inside tick if statement for PursuePath" << std::endl;
		if(theGrid->getPos(target->getPosition()) != path.back())
		{
			//Find the new path if the player has moved
			path = theGrid->findPath(theGrid->getPos(entity->ogreSceneNode->getPosition()),
					theGrid->getPos(target->getPosition()));
		} else
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
}

bool PursuePath::done()
{
	if(path.empty())
	{
		entity->speed = 0;
		return true;
	} else
	{
		return false;
	}
}

SearchPath::SearchPath(Entity381 *ent, Ogre::Vector3 location) : Command(ent, COMMAND_TYPE::SEARCHPATH)
{
	std::cout << "SearchPath made" << std::endl;
	targetLocation = location;
	MOVE_DISTANCE_THRESHOLD = 100;

	theGrid = NULL;
	init();
}

SearchPath::~SearchPath()
{

}

void SearchPath::init()
{
	theGrid = entity->engine->gameMgr->grid;
	path = theGrid->findPath(theGrid->getPos(entity->ogreSceneNode->getPosition()),
			theGrid->getPos(targetLocation));

	std::cout << "Path length: " << path.size() << std::endl;
}

void SearchPath::tick(float dt)
{

	std::cout << "Made it inside SearchPath tick" << std::endl;
	if(!path.empty())
	{
		std::cout << "Made it inside if in tick for SearchPath" << std::endl;
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
		entity->speed = 0;
		return true;
	} else
	{
		return false;
	}
}







