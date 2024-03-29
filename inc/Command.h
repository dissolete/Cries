/*
 * Command.h
 *
 *  Created on: Mar 28, 2017
 *      Author: sushil
 */

#ifndef COMMAND_H_
#define COMMAND_H_

class Entity381;

#include <Entity381.h>
#include <OgreVector3.h>
#include <Types.h>
#include <Grid.h>
#include <Utils.h>

class Command {

public:
	Command(Entity381 *ent, COMMAND_TYPE ct);
	virtual ~Command();

	virtual void init();
	virtual void tick(float dt);
	virtual bool done();

	Entity381* entity;
	COMMAND_TYPE commandType;

};

class Search: public Command {

public:
	Search(Entity381* ent);
	~Search();

	void init();
	void tick(float dt);
	bool done();

	bool isDone;

	float MOVE_DISTANCE_THRESHOLD;

};

class Pursue: public Command {
public:
	Pursue(Entity381* ent, Ogre::SceneNode* targ);
	~Pursue();

	virtual void init();
	virtual void tick(float dt);
	virtual bool done();


	Ogre::SceneNode* target;

};

class PursuePath : public Command {
public:
	PursuePath(Entity381 *ent, Ogre::SceneNode *targ);
	~PursuePath();

	virtual void init();
	virtual void tick(float dt);
	virtual bool done();

	std::list<GridParams*> path;
	Ogre::SceneNode* target;
	Grid *theGrid;
};

class SearchPath: public Command {

public:
	SearchPath(Entity381* ent, Ogre::Vector3 location);
	~SearchPath();

	void init();
	void tick(float dt);
	bool done();

	Ogre::Vector3 targetLocation;
	float MOVE_DISTANCE_THRESHOLD;

	std::list<GridParams*> path;
	Grid *theGrid;

};

#endif /* COMMAND_H_ */
