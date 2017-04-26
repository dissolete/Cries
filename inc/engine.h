/*
 * engine.h
 *
 *  Created on: Mar 9, 2017
 *      Author: sushil
 */

#ifndef ENGINE_H_
#define ENGINE_H_

class InputMgr;

#include <GfxMgr.h>
#include <inputMgr.h>
#include <EntityMgr.h>
#include <gameMgr.h>
#include <Types.h>
#include <UiMgr.h>

class EntityMgr;

class Engine {
private:
public:
	Engine();
	~Engine();


	EntityMgr*      entityMgr;
	GfxMgr*         gfxMgr;
	InputMgr*       inputMgr;
	GameMgr*        gameMgr;
	UiMgr *uiMgr;
	//ControlMgr*   controlMgr;

	void init();
	void run();
	void tickAll(float dt);
	void stop();
	void shutdown();
	void loadLevel();

	//
	bool keepRunning;

	STATE theState;//Stores the current state that the engine is in

	float timeSinceLastEvent;//Stores the time (usually) since last state change


};



#endif /* ENGINE_H_ */
