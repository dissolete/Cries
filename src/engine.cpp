/*
 * engine.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: sushil
 */

#include <engine.h>
#include <OgreTimer.h>

Engine::Engine(){
	gfxMgr = 0;
	inputMgr = 0;
	entityMgr = 0;
	gameMgr = 0;
	uiMgr = 0;
	keepRunning = true;
	theState = STATE::SPLASH;
	timeSinceLastEvent = 0;

}

Engine::~Engine(){

}

void Engine::stop(){
	keepRunning = false;
}

void Engine::init(){
// construct
	gfxMgr    = new GfxMgr(this);
	std::cout << "Constructed GfxMgr" << std::endl;
	inputMgr = new InputMgr(this);
	entityMgr = new EntityMgr(this);
	gameMgr = new GameMgr(this);
	uiMgr = new UiMgr(this);

	// initialize
	gfxMgr->init();
	inputMgr->init();
	entityMgr->init();
	gameMgr->init();
	uiMgr->init();

// load level to play
	gfxMgr->loadLevel();
	inputMgr->loadLevel();
	entityMgr->loadLevel();
	gameMgr->loadLevel();
	uiMgr->loadLevel();

}

void Engine::tickAll(float dt){
	gfxMgr->tick(dt);
	inputMgr->tick(dt);
	entityMgr->tick(dt);
	gameMgr->tick(dt);
	uiMgr->tick(dt);
}

void Engine::shutdown(){
	inputMgr->stop();
	gfxMgr->stop();
	entityMgr->stop();
	gameMgr->stop();
	uiMgr->stop();
}

void Engine::run(){
	Ogre::Timer* timer = new Ogre::Timer();
	std::cout << "Got ogre timer" << std::endl;
	float oldTime = timer->getMilliseconds()/1000.0f;
	std::cout << "Time: " << oldTime << std::endl;
	float newTime;
	float dt = 0.001f;
	std::cout << "Entering while loop" << std::endl;
	while(keepRunning){
		newTime = timer->getMilliseconds()/1000.0f;
		dt = newTime - oldTime;
		oldTime = newTime;
		tickAll(dt);
	}
	shutdown();

	return;
}
