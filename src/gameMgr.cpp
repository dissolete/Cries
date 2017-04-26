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
<<<<<<< HEAD
   this->loadLevel("level001.txt");
=======
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
>>>>>>> 34af3717c008466a6e28279555e689b3a3cf3fff
}

void GameMgr::stop(){

}

void GameMgr::tick(float dt){

}

void GameMgr::createEnts(){

}

void GameMgr::createGround(int &width, int &heigth, std::string &material)
{
	// Create Plane
	Ogre::MeshManager::getSingleton().createPlane(
	    "ground",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    ocean,
		width, heigth, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);

	// Create Ground Entity
	Ogre::Entity* groundEntity = engine->gfxMgr->ogreSceneManager->createEntity("ground");

	engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

<<<<<<< HEAD
	groundEntity->setCastShadows(false);

	groundEntity->setMaterialName(material);
=======

>>>>>>> 9b0c2777de4349ac5b112ecea59e129c1e5edfac
}

void GameMgr::createSky(){

	engine->gfxMgr->ogreSceneManager->setSkyBox(true, "Examples/MorningSkyBox");

}

void GameMgr::loadLevel(std::string levelFilename)
{
	// Load the environment, objects, and characters
	this->loadEnvironment(levelFilename);
	this->setupEnvironment();
	this->loadObjects();
	this->loadCharacters();
}

void GameMgr::loadEnvironment(std::string levelFilename)
{
	// Variables
	std::ifstream fin;
	int x, z;
	std::string groundMaterial;
	std::string buffer;


	// Open File
	fin.open(levelFilename);

	// Check for bad file
	if(!fin.is_open())
	{
		std::cerr << "ERROR, FILE CANNOT BE OPENED" << std::endl;
		return;
	}

	// First block is world dimensions and material name
	fin >> x >> z;
	fin >> groundMaterial;

	// TESTING FILE READ ////////////////////////////////////////////
	std::cerr << "Ground Dimensions: " << x << " x " << z << std::endl;
	std::cerr << "Ground Material: " << groundMaterial << std::endl;
	/////////////////////////////////////////////////////////////////

	// Create floor mesh with read in dimensions
	createGround( x, z, groundMaterial);

	// Second block reads in location of you and enemies



	// Create the Entities
}

void GameMgr::setupEnvironment()
{
	//We know graphicsMgr is ready and initialized
	engine->gfxMgr->ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = engine->gfxMgr->ogreSceneManager->createLight("MainLight");
	light->setPosition(20.0, 80.0, 50.0);
}

void GameMgr::loadObjects()
{

}

void GameMgr::loadCharacters()
{

}






