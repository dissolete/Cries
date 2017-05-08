/*
 * EntityMgr.h
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#ifndef ENTITYMGR_H_
#define ENTITYMGR_H_


class Entity381;

#include <list>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <Types.h>

#include <Entity381.h>
#include <mgr.h>
#include <Utils.h>

class Engine;

class EntityMgr: public Mgr {

private:

	//
public:

	Entity381* selectedEntity;
	std::list<Entity381 *> entities;

	EntityMgr(Engine *eng);

	~EntityMgr();

	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	Entity381 *CreateEntity(EntityType entityType, Ogre::Vector3 pos, float yaw = 0);
	void CreateOgreEntityAndNode(Entity381 *ent);
	void fixCollisions(Entity381 *ent);
};

class readFromFile {
public:
	std::string mesh;
	Ogre::Vector3 positionOffset;
	float entityScale;
	float entityOrientation;
	bool entity;
};



#endif /* ENTITYMGR_H_ */
