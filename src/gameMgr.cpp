/*
 * gameMgr.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#include <gameMgr.h>
#include <engine.h>
#include <OgreMeshManager.h>

GameMgr::GameMgr(Engine *engine): Mgr(engine), entitySceneNodes(){
	floor = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    ceiling = new Ogre::MovablePlane("ceiling");
    ceiling->d = 0;
    ceiling->normal = -1 * Ogre::Vector3::UNIT_Y;
    gameplayTime = 0;
    entityCount = 0;
}

GameMgr::~GameMgr(){

}

void GameMgr::init(){

}

void GameMgr::loadLevel(){
	this->loadLevel("level001.txt");

}

void GameMgr::stop(){

}

void GameMgr::tick(float dt){
	//engine->gfxMgr->ogreAnimationState->addTime(dt);
	if(engine->theState == STATE::GAMEPLAY)
	{
		gameplayTime += dt;
		std::cout << "Gameplay time: " << gameplayTime << std::endl;
	}
}


void GameMgr::createGround(int width, int heigth, std::string &material)
{
	// Create Plane //////////////////////////////////////////////////////////////////////////////////////////
	Ogre::MeshManager::getSingleton().createPlane(
	    "ground",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    floor,
		width, heigth, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create Ground Entity //////////////////////////////////////////////////////////////////////////////////
	Ogre::Entity* groundEntity = engine->gfxMgr->ogreSceneManager->createEntity("ground");
	engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName(material);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create Separated Water Ground ////////////////////////////////////////////////////////////////////////
	Ogre::Entity* groundEntity2 = engine->gfxMgr->ogreSceneManager->createEntity("ground");
	engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity2);
	groundEntity2->setCastShadows(false);
	groundEntity2->setMaterialName("Examples/WaterStream");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void GameMgr::createCeiling()
{
	Ogre::MovablePlane plane(-Ogre::Vector3::UNIT_Y, 100);

	// Create Ceiling ///////////////////////////////////////////////////////////////////////////////////////
	Ogre::MeshManager::getSingleton().createPlane(
	    "ceiling",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		4000, 3400, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Create Ceiling Entity //////////////////////////////////////////////////////////////////////////////////
	Ogre::Entity* ceiling = engine->gfxMgr->ogreSceneManager->createEntity("ceiling");
	engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ceiling);
	ceiling->setCastShadows(false);
	ceiling->setMaterialName("Examples/Rockwall");
}

void GameMgr::createSky(){

	engine->gfxMgr->ogreSceneManager->setSkyBox(true, "Examples/SpaceSkyBox");

}

void GameMgr::loadLevel(std::string levelFilename)
{
	// Load the environment, objects, and characters
	this->loadEnvironment(levelFilename);
	this->setupEnvironment();
	this->setupSounds();
	this->loadObjects();
	this->loadCharacters();
}

void GameMgr::loadEnvironment(std::string levelFilename)
{
	// Variables
	std::ifstream fin;
	int x, z;
	float x_offset, y_offset, z_offset;
	float scale, orientation;
	std::string groundMaterial, objectMesh, characterMesh;
	std::string buffer, buff;
	char objectChar;
	std::map<std::string, readFromFile*> objects;


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
	createGround( x*1000, z*1000, groundMaterial);

	// Create Ceiling
	createCeiling(); //DEBUG THIS LATER

	// Setup the grid
	this->grid = new Grid( engine->gfxMgr->ogreSceneManager, x, z, engine);

	// Second block reads in location of you and enemies
	// Check for Objects line
	fin >> buffer;

	if( buffer != "Objects" )
	{
		std::cerr << "FILE NOT FORMATTED CORRECTLY" << std::endl;
	}

	// Read Objects and positions
	readFromFile* readEnt = new readFromFile();

	fin >> buff; // read single char

	fin >> readEnt->mesh;
	fin >> x_offset >> y_offset >> z_offset;
	fin >> readEnt->entityOrientation >> readEnt->entityScale;

	// Set entity info
	readEnt->positionOffset = Ogre::Vector3(x_offset, y_offset, z_offset);
	readEnt->entity = false;
	objects[buff] = readEnt;

	readEnt = new readFromFile(); // read in next if any

	// Testing Second Box Readin ///////////////////////////////////
	std::cerr << "Objects" << std::endl;
	std::cerr << objectChar << " " << objectMesh << std::endl;
	std::cerr << "Located at: " << x_offset << ", " << y_offset << ", " << z_offset << std::endl;
	std::cerr << "Scaled at: " << orientation << " " << scale << std::endl;
	///////////////////////////////////////////////////////////////


	// Check for Characters line /////////////////////////////////////////////////////////////////
	fin >> buffer;

	if( buffer != "Characters" )
	{
		std::cerr << "FILE NOT FORMATTED CORRECTLY" << std::endl;
	}

	// Read in Characters

	fin >> buff; // read next char

	fin >> readEnt->mesh;
	fin >> x_offset >> y_offset >> z_offset;
	fin >> readEnt->entityOrientation >> readEnt->entityScale;

	// set entity info
	readEnt->positionOffset = Ogre::Vector3(x_offset, y_offset, z_offset);
	readEnt->entity = true;
	objects[buff] = readEnt;

	// read next if any
	readEnt = new readFromFile();

	// Testing Third Box Readin ///////////////////////////////////
	std::cerr << "Characters" << std::endl;
	std::cerr << objectChar << " " << characterMesh << std::endl;
	std::cerr << "Located at: " << x_offset << ", " << y_offset << ", " << z_offset << std::endl;
	std::cerr << "Scaled at: " << orientation << " " << scale << std::endl;
	///////////////////////////////////////////////////////////////

	// Read the World Placement //////////////////////////////////////////////////////////////////
	char c;

	// Start to see if world is in file
	fin >> buffer;

	if( buffer != "World" )
	{
		std::cerr << "FILE NOT FORMATTED CORRECTLY" << std::endl;
	}


	// Pre Conditions for World setup
	/*
	Ogre::Vector3 wallPosition;
	wallPosition = Ogre::Vector3(-1850, 50, 1200);

	Ogre::Vector3 archPosition;
	archPosition = Ogre::Vector3(0, 0, 0);

	Ogre::Vector3 enemyPosition;
	enemyPosition = Ogre::Vector3(-1200, 0, 1000);
	*/

	std::ofstream fout;
	fout.open("GRID.txt");

	// Loop through map dimensions
	for( int row = 0; row < ROW_SIZE; row++ )
	{
		for( int col = 0; col < COL_SIZE; col++ )
		{
			fin >> c;

			fout << c; // test world read in

			Ogre::Vector3 gridPositionInWorld = this->grid->getPosition(row, col);


			if( c == 'P')
			{
				// Find the Start Position and set camera to there
				engine->gfxMgr->setCameraPosition( gridPositionInWorld );
				std::cerr << gridPositionInWorld << std::endl;
			}


			// Check for walls (Not player or enemy nodes)
			if( c == 'W' )
			{

				// Grab the "objects" config stuff from the object map that Hadi built
				readFromFile * objectEntData = objects["D"]; // Currently not used lmao

				GridParams * gridParam =  this->grid->getGrid(row, col);
				if(gridParam) gridParam->notWalkable();

				engine->entityMgr->CreateEntity(EntityType::WALL, gridPositionInWorld, 0);

				objectEntData = NULL;
				gridParam = NULL;
			}


			// Check for Arch
			else if( c == 'A' )
			{
//				std::cerr << "Spawning Arch" << std::endl;
//				engine->entityMgr->CreateEntity(EntityType::ARCH, archPosition, 0);
//				archPosition.x += 50;
				readFromFile * objectEntData = objects["C"]; // Currently not used lmao

				GridParams * gridParam =  this->grid->getGrid(row, col);
				if(gridParam) gridParam->notWalkable();

				engine->entityMgr->CreateEntity(EntityType::ARCH, gridPositionInWorld, 0);

				objectEntData = NULL;			std::ofstream fout;
				gridParam = NULL;

			}

			// See no evil
			else if( c == 'C' )
			{
				engine->entityMgr->CreateEntity(EntityType::SEENO, gridPositionInWorld, 0);
			}

			else if(c == 'S')
			{
				engine->entityMgr->CreateEntity(EntityType::SPEAKNO, gridPositionInWorld, 0);
			}
			else if(c == 'H')
			{
				engine->entityMgr->CreateEntity(EntityType::HEARNO, gridPositionInWorld, 0);
			}
		}

		fout << std::endl;
	}

	fout.close();
	// Create the Entities

	// Create Skybox for the hell of it
	createSky();
//	delete readEnt;
}

void GameMgr::setupEnvironment()
{
	//We know graphicsMgr is ready and initialized
	engine->gfxMgr->ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
	Ogre::Light* light = engine->gfxMgr->ogreSceneManager->createLight("MainLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(100.0, 800.0, 100.0);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue::White);
}

/*
 *
 * Basic Setup for pre-alpha testing
 * Will be more complex in later phases
 *
 */
void GameMgr::setupSounds()
{
	// Load Song from file
	engine->soundMgr->load_song("Layer 1", "/home/hrumjahn/git/Cries/resources/ss.wav");
	//load_sound(std::string soundName, std::string filePath);

	//play_sound(std::string soundName);
	engine->soundMgr->play_song("Layer 1", true);

	// Play song (.wav)
}

void GameMgr::loadObjects()
{
	// Create Entity
	Ogre::Entity *splash = engine->gfxMgr->ogreSceneManager->createEntity("Splash.mesh");

	// Create scene node for this entity
	engine->gfxMgr->splashNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode();
	engine->gfxMgr->splashNode->attachObject(splash);
	splash->setMaterialName("Material");
	engine->gfxMgr->splashNode->setScale(10.f, 10.0f, 10.0f);
	engine->gfxMgr->splashNode->setPosition( 0.0f, 400, -3500);
	engine->gfxMgr->splashNode->roll(Ogre::Degree(-360));
	engine->gfxMgr->splashNode->pitch(Ogre::Degree(90));
}

void GameMgr::loadCharacters()
{
	//Creating the entities
//	engine->entityMgr->CreateEntity(EntityType::HEARNO, Ogre::Vector3(0, 10, -1000), 0);
//	engine->entityMgr->CreateEntity(EntityType::SEENO, Ogre::Vector3(500, 0, -500), Ogre::Math::HALF_PI / 2);
//	engine->entityMgr->CreateEntity(EntityType::SPEAKNO, Ogre::Vector3(-500, 20, -500), Ogre::Math::HALF_PI / -2);
/*
	// Set Animation States
	//engine->gfxMgr->ogreAnimationState = splash->getAnimationState("Test");
	engine->gfxMgr->ogreAnimationState->setWeight(1);
	engine->gfxMgr->ogreAnimationState->setLoop(true);
	engine->gfxMgr->ogreAnimationState->setEnabled(true);
*/


}

std::string GameMgr:: getNewName()
{

	return std::string("object_" + std::to_string(entityCount++));
}



