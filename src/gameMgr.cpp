/*
 * gameMgr.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#include <gameMgr.h>
#include <engine.h>
#include <OgreMeshManager.h>

GameMgr::GameMgr(Engine *engine): Mgr(engine){
	ocean = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
}

GameMgr::~GameMgr(){

}

void GameMgr::init(){

}

void GameMgr::loadLevel(){
//We know graphicsMgr is ready and initialized
	engine->gfxMgr->ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = engine->gfxMgr->ogreSceneManager->createLight("MainLight");
	light->setPosition(20.0, 80.0, 50.0);

	createSky();
	createGround();

	//Creating the entities
	engine->entityMgr->CreateEntity(EntityType::HEARNO, Ogre::Vector3(0, 10, -1000), 0);
	engine->entityMgr->CreateEntity(EntityType::SEENO, Ogre::Vector3(500, 0, -500), Ogre::Math::HALF_PI / 2);
	engine->entityMgr->CreateEntity(EntityType::SPEAKNO, Ogre::Vector3(-500, 20, -500), Ogre::Math::HALF_PI / -2);
}

void GameMgr::stop(){

}

void GameMgr::tick(float dt){

}

void GameMgr::createEnts(){

}

void GameMgr::createGround(){

	//Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	  Ogre::MeshManager::getSingleton().createPlane(
	    "ground",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    ocean,
	    15000, 15000, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);

	  Ogre::Entity* groundEntity = engine->gfxMgr->ogreSceneManager->createEntity("ground");
	  engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	  groundEntity->setCastShadows(false);
	  //	  groundEntity->setMaterialName("Ocean2_HLSL_GLSL");
	  //groundEntity->setMaterialName("OceanHLSL_GLSL");
	  groundEntity->setMaterialName("Ocean2_Cg");
	  //groundEntity->setMaterialName("NavyCg");


}

void GameMgr::createSky(){

	engine->gfxMgr->ogreSceneManager->setSkyBox(true, "Examples/MorningSkyBox");

}





