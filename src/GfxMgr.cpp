/*
 * GfxMgr.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: sushil
 */

#include <GfxMgr.h>
#include <engine.h>
#include <unistd.h>

GfxMgr::GfxMgr(Engine *eng): Mgr(eng) {
	rect = NULL;
	backgroundNode = NULL;

	resources = "resources.cfg";
	plugins   = "plugins.cfg";

	ogreRoot = new Ogre::Root(plugins);

	cf.load(resources);
	loadConfig(cf);
	configureRenderSystem();
	ogreRenderWindow = ogreRoot->initialise(true, "CRIES");
	createSceneManager();
	createCamera();
	createViewport();

}

void GfxMgr::loadConfig(Ogre::ConfigFile cf){
	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
	while(secIt.hasMoreElements()){
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;
		for(it = settings->begin(); it != settings->end(); ++it){
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}
}

void GfxMgr::configureRenderSystem(){
	if(!(ogreRoot->restoreConfig() || ogreRoot->showConfigDialog()))
		engine->stop();
}

void GfxMgr::initResources(){
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GfxMgr::createSceneManager(){
	ogreSceneManager = ogreRoot->createSceneManager(Ogre::ST_GENERIC);
}

void GfxMgr::createCamera(){

	// Set up for first-person camera follows

	// Create main camera
	ogreCamera = ogreSceneManager->createCamera("MainCam");
	ogreCamera->setPosition(0, 20, 0);
	ogreCamera->setNearClipDistance(5);
	cameraNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();

	// Create yaw node
	yawNode = cameraNode->createChildSceneNode();

	// Create pitch node
	pitchNode = yawNode->createChildSceneNode();
	pitchNode->attachObject(ogreCamera);


	//cameraNode->setPosition(0, 100, 500);
	//ogreCamera->lookAt(0, 0, 0);

}

void GfxMgr::createViewport(){
	ogreViewport = ogreRenderWindow->addViewport(ogreCamera);
	ogreViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	ogreCamera->setAspectRatio(Ogre::Real(ogreViewport->getActualWidth()) /
											Ogre::Real(ogreViewport->getActualHeight()));


	//begin background testing
	// Create background material

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("splash.png");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(true);

	// Testing Animated Splash Screen
	//engine->entityMgr->CreateEntity(EntityType::LOGO, Ogre::Vector3(0, 0, 0), 0);
/*
	Ogre::Entity *splash = ogreSceneManager->createEntity("Logo", "boat.mesh");
	splash->setCastShadows(true);

	// Create scene node for this entity
	splashNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode("Logo");
	splashNode->attachObject(splash);
	splash->setMaterialName("DDG51.bmp");
	splashNode->setScale(10.f, 10.0f, 10.0f);

	// Set Animation States
	ogreAnimationState = splash->getAnimationState("CameraRotate");
	ogreAnimationState->setLoop(true);
	ogreAnimationState->setEnabled(true);
*/


	// Create background rectangle covering the whole screen
	rect = new Ogre::Rectangle2D(true);
	rect->setCorners(-1.0, 1.0, 1.0, -1.0);
	rect->setMaterial("Background");

	// Render the background before everything else
	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	// Attach background to the scene
	backgroundNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode("Background");
	backgroundNode->attachObject(rect);

}

GfxMgr::~GfxMgr() {
	delete ogreRoot; //after inputMgr destructor
	delete rect;
	rect = NULL;
}

void GfxMgr::init(){
	std::cout << "Initialize gfx" << std::endl;
	initResources();

}

void GfxMgr::loadLevel(){

	backgroundNode->detachObject(rect);//Removes the background so it is no longer rendered

}


void GfxMgr::tick(float dt){
	Ogre::WindowEventUtilities::messagePump();
	//if(ogreRenderWindow->isClosed()) engine->stop();
	if(!ogreRoot->renderOneFrame()) engine->stop();

	if(engine->theState == STATE::SPLASH)
	{
		//Sets the lighting to be dim then bright then dim until the level loads according to the formula
		//lightVal = -0.4222(time^2) + 0.95
		float lightVal = (-0.4222f * (engine->timeSinceLastEvent - 1.5f) * (engine->timeSinceLastEvent - 1.5f)) + 0.95f;
		ogreSceneManager->setAmbientLight(Ogre::ColourValue(lightVal, lightVal, lightVal));
	}


	return;
}

void GfxMgr::stop(){
	std::cout << "stopping engine and ogre" << std::endl;
	ogreRoot->shutdown();
	return;
}

void GfxMgr::loadMenu()
{
	// Create background material
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("MenuScreen", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("GameLogoPrototype.png");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	rect->setMaterial("MenuScreen");
}
