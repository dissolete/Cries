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


Search::Search(Entity381 * ent): Command(ent, COMMAND_TYPE::SEARCH){

	MOVE_DISTANCE_THRESHOLD = 100;
	isDone = false;
}

Search::~Search(){

}

void Search::init(){

}

bool Search::done(){
	if(isDone)
	{
		entity->SetStatus(Status::WAITING);
	}

	return isDone;
}

void Search::tick(float dt){

	/*Ogre::Vector3 diff = targetLocation - entity->pos;
	entity->desiredHeading  = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;*/
	Ogre::Vector3 vel;
	vel.x = entity->speed * Ogre::Math::Cos(entity->heading);
	vel.z = entity->speed * Ogre::Math::Sin(entity->heading);
	//If the tick will put a wall within the collision range of the entity, we are done
	Ogre::Vector3 frontEdge = entity->pos + entity->collisionRange + entity->speed * dt * vel;
	GridParams *endCord = entity->engine->gameMgr->grid->getPos(frontEdge);
	if(endCord == NULL || !endCord->isWalkable())
	{
		std::cout << "Made it iside if statement for search tick" << std::endl;
		entity->speed = 0;
		isDone = true;
		//Have the entity stop and turn in place all the way around
		std::cout << "Made it before the FixAngle call" << std::endl;
		entity->desiredHeading = FixAngle(entity->heading + Ogre::Math::PI);
		std::cout << "Made it after FixAngle call" << std::endl;

	}
}

Pursue::Pursue(Entity381 *ent, Ogre::SceneNode* targ): Command(ent, COMMAND_TYPE::PURSUE){
	target = targ;
	init();
}

Pursue::~Pursue(){

}

void Pursue::init(){
	Ogre::Vector3 diff = target->getPosition() - entity->pos;
	entity->desiredHeading = atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;
}

bool Pursue::done(){
	Ogre::Vector3 diff = target->getPosition() - entity->pos;
	diff.y = 0;//Don't want to include height difference
	if(diff.length() <= 5)
	{
		entity->speed = 0;
		entity->SetStatus(Status::WAITING);
		std::cout << "Pursue done" << std::endl;
		//end game here
		return true;
	} else
	{
		return false;
	}
}

void Pursue::tick(float dt){
	//compute offset
	Ogre::Vector3 diff = target->getPosition() - entity->pos;
	//Use if statement to prevent rapidly going back and forth when close to the player
	/*if(diff.length() > 200.0)
	{
		entity->desiredHeading = atan2(diff.z, diff.x);
	}*/
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
	targetLocation = targetLocation = location;
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







