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
	floor = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    ceiling = new Ogre::MovablePlane("ceiling");
    ceiling->d = 0;
    ceiling->normal = -1 * Ogre::Vector3::UNIT_Y;
    gameplayTime = 0;
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


void GameMgr::createGround(int &width, int &heigth, std::string &material)
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
	Ogre::MovablePlane plane(-1 * Ogre::Vector3::UNIT_Y, 50);

	// Create Ceiling ///////////////////////////////////////////////////////////////////////////////////////
	Ogre::MeshManager::getSingleton().createPlane(
	    "ceiling",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		4000, 3400, 20, 20,
	    true,
	    1, 5, 5,
	    -Ogre::Vector3::UNIT_Z);
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
	std::string buffer;
	char objectChar;


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

	// Create Ceiling
	createCeiling(); //DEBUG THIS LATER

	// Second block reads in location of you and enemies
	// Check for Objects line
	fin >> buffer;

	if( buffer != "Objects" )
	{
		std::cerr << "FILE NOT FORMATTED CORRECTLY" << std::endl;
	}

	// Read Objects and positions
	fin >> objectChar >> objectMesh;
	fin >> x_offset >> y_offset >> z_offset;
	fin >> orientation >> scale;

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
	fin >> objectChar >> characterMesh;
	fin >> x_offset >> y_offset >> z_offset;
	fin >> orientation >> scale;

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
	Ogre::Vector3 wallPosition;
	wallPosition = Ogre::Vector3(-1850, 50, 1200);

	Ogre::Vector3 archPosition;
	archPosition = Ogre::Vector3(0, 0, 0);

	Ogre::Vector3 enemyPosition;
	enemyPosition = Ogre::Vector3(-1200, 0, 1000);

	/*
	Ogre::ManualObject test;
	test.convertToMesh(objectMesh, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	*/

	// Loop through map dimensions
	for( int row = 0; row < ROW_SIZE; row++ )
	{
		for( int col = 0; col < COL_SIZE; col++ )
		{
			fin >> c;
			std::cerr << c << std::endl;


			// Check for walls (Not player or enemy nodes)
			if( c == 'W' )
			{
				std::cerr << "Creating Wall" << std::endl;

				engine->entityMgr->CreateEntity(EntityType::WALL, wallPosition, 0);
				std::cerr << "Wall Created" << std::endl;
				//engine->entityMgr->

				// Create a Wall
				/*
				Ogre::Entity* wallEntity = engine->gfxMgr->ogreSceneManager->createEntity(getNewName(), objectMesh);
				wallEntity->setMaterialName("Examples/RustySteel");
				engine->gfxMgr->wallNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode(wallPosition);
				engine->gfxMgr->wallNode->attachObject(wallEntity);
				*/


				// Set Scale and position
				//engine->gfxMgr->ogreSceneManager->getRootSceneNode()->setScale(0.1f, 1.0f, 0.1f);
				//engine->gfxMgr->ogreSceneManager->getRootSceneNode()->setPosition(wallPosition);

				// Check wall position to prevent going outside of map
				if( wallPosition.x < 1800 ||
					wallPosition.x > -1800 ||
					wallPosition.z > -2500 ||
					wallPosition.z < 2500 )
				{
					// reset position
					//wallPosition.x = 0;
					//wallPosition.z = 0;

					// Increment Positions to prevent overlap from reset
					wallPosition.x += 25;
					//wallPosition.z += 50;
				}

				//wallPosition.z += 50;

				// Set blocked area
			}


			// Check for Arch
			else if( c == 'A' )
			{
				std::cerr << "Spawning Arch" << std::endl;
				engine->entityMgr->CreateEntity(EntityType::ARCH, archPosition, 0);
				archPosition.x += 50;

			}


			else if( c == 'C' )
			{
				// Spawn Enemy
				std::cerr << "Spawning Enemy" << std::endl;

				loadCharacters();
/*
				// Create Test Enemy
				Ogre::Entity* enemyEntity = engine->gfxMgr->ogreSceneManager->createEntity(getNewName(), characterMesh);
				// enemyEntity->setMaterialName("Examples/RustySteel");

				engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode(enemyPosition)->attachObject(enemyEntity);


				// Set Scale and position
				engine->gfxMgr->ogreSceneManager->getRootSceneNode()->setScale(0.1f, 0.02f, 0.1f);
				//engine->gfxMgr->ogreSceneManager->getRootSceneNode()->setPosition(position);

				enemyPosition.x += 10; // Increment Positions to prevent overlap
				//enemyPosition.z += 50;
*/
			}
		}
	}

	// Create the Entities

	// Create Skybox for the hell of it
	createSky();
}

void GameMgr::setupEnvironment()
{
	//We know graphicsMgr is ready and initialized
	engine->gfxMgr->ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
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
	// Future use to load other non moving objects into the world
}

void GameMgr::loadCharacters()
{
	//Creating the entities
	engine->entityMgr->CreateEntity(EntityType::HEARNO, Ogre::Vector3(0, 10, -1000), 0);
	engine->entityMgr->CreateEntity(EntityType::SEENO, Ogre::Vector3(500, 0, -500), Ogre::Math::HALF_PI / 2);
	engine->entityMgr->CreateEntity(EntityType::SPEAKNO, Ogre::Vector3(-500, 20, -500), Ogre::Math::HALF_PI / -2);

	// Setup Animation Defaults
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RIM_LINEAR);


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
	static int count = 0;

	std::string s;
	std::stringstream out;
	out << count ++;
	s = out.str();

	return "object_" + s;
}



