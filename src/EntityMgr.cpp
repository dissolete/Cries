/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */


#include <EntityMgr.h>
#include <engine.h>


EntityMgr::EntityMgr(Engine *eng): Mgr(eng){
	entities.clear();
	selectedEntity = 0;
}

EntityMgr::~EntityMgr(){
	entities.clear();
}

void EntityMgr::tick(float dt){
	for (std::list<Entity381 *>::const_iterator it = entities.begin(); it != entities.end(); ++it){
		(*it)->Tick(dt);
	}
}

void EntityMgr::init(){
	return;
}

void EntityMgr::loadLevel(){

}

void EntityMgr::stop(){
	return;
}

void EntityMgr::CreateOgreEntityAndNode(Entity381 *ent){

	if(ent) {
		ent->ogreEntity = engine->gfxMgr->ogreSceneManager->createEntity(ent->meshfile);
		ent->ogreSceneNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode(ent->pos);
		ent->ogreSceneNode->attachObject(ent->ogreEntity);
		//ent->ogreEntity->setCastShadows(true);
		ent->ogreSceneNode->yaw(Ogre::Radian(ent->heading + Ogre::Math::PI/2));
	}

}

Entity381* EntityMgr::CreateEntity(EntityType entityType, Ogre::Vector3 position, float heading){

	Entity381 *ent = 0;// = new Entity381(entityType, position, heading);
	switch(entityType){
	case EntityType::HEARNO :
		ent = new HearNo(position, heading, engine);
		break;
	case EntityType::SEENO:
		ent = new SeeNo(position, heading, engine);
		break;
	case EntityType::SPEAKNO:
		ent = new SpeakNo(position, heading, engine);
		break;

	// Set up wall parameters
	case EntityType::WALL:
		ent = new Wall(position, heading, engine);
		break;

	case EntityType::ARCH:
		ent = new Arch(position, heading, engine);
		break;

	// TESTING
	case EntityType::LOGO:
		ent = new Logo(position, heading, engine);
		break;

	default:
		ent = new HearNo(position, heading, engine);
		break;
	}

	if( entityType == EntityType::LOGO )
	{
		/*
		//ent->ogreEntity->setMaterialName("splash");
		ent->engine->gfxMgr->splashNode->setScale(10.f, 10.0f, 10.0f);
		ent->engine->gfxMgr->ogreAnimationState = ent->ogreEntity->getAnimationState("CameraRotate");
		ent->engine->gfxMgr->ogreAnimationState->setLoop(true);
		ent->engine->gfxMgr->ogreAnimationState->setEnabled(true);
		*/
	}

	CreateOgreEntityAndNode(ent);

	if( entityType == EntityType::ARCH )
	{
		ent->ogreSceneNode->yaw(Ogre::Degree(90));
	}

	if( entityType == EntityType::HEARNO )
	{
		//ent->ogreSceneNode->set
	}

	if( entityType == EntityType::SEENO )
	{
		//ent->ogreSceneNode->yaw(Ogre::Radian(ent->heading + Ogre::Math::PI/2));
	}

	if( entityType == EntityType::SPEAKNO )
	{

	}

	// Check material name
	if( entityType == EntityType::WALL )
	{
		ent->ogreEntity->setMaterialName("moss");
		ent->ogreSceneNode->setScale(1, 10, 1);
	}

	entities.push_front(ent);
	return ent;
}

//Will not handle the situation where it collides with two entities on opposite sides of each other
void EntityMgr::fixCollisions(Entity381 *ent)
{
	for(Entity381 *other : entities)
	{
		//If the entity is not itself, and the entity is not an arch, check for collisions
		if(ent != other && ent->entityType != EntityType::ARCH)
		{
			Ogre::Vector3 offset = other->pos - ent->pos;
			//If the offset's length is less than the sum of their collision ranges, move back
			if(ent->collides(other))
			{
				//Find how far you must offset the entity to remove it from the other entity
				setMagnitude(offset, (ent->collisionRange + other->collisionRange - offset.length()));
				ent->ogreSceneNode->translate(offset, Ogre::Node::TS_LOCAL);
				ent->pos = ent->pos + offset;
			}
		}
	}
}

