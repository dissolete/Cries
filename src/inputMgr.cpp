/*
 * inputMgr.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#include <inputMgr.h>
#include <engine.h>
#include <cfloat>
#include <Command.h>
#include <Aspect.h>
#include <UnitAI.h>
#include <UiMgr.h>

#include <string>
#include <iostream>
#include <sstream>
#include <ostream>

#include <OgreRay.h>


InputMgr::InputMgr(Engine *engine) : Mgr(engine){
	keyboardTimer = keyTime;
	selectionTimer = selectionTime;
	selectionDistanceSquaredThreshold = 10000;

	keyboard = 0;
	mouse = 0;
	oisInputManager = 0;

	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;
	engine->gfxMgr->ogreRenderWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;

	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

	// insert the following lines right before calling mInputSystem = OIS::InputManager::createInputSystem( paramList );
#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("true")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("true")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	oisInputManager = OIS::InputManager::createInputSystem(pl);
	keyboard = static_cast<OIS::Keyboard*>(oisInputManager->createInputObject(OIS::OISKeyboard, true));
	mouse    = static_cast<OIS::Mouse*>(oisInputManager->createInputObject(OIS::OISMouse, true));

	keyboard->setEventCallback(this);
	mouse->setEventCallback(this);

	windowResized(engine->gfxMgr->ogreRenderWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);

	isSprinting = false;

}

InputMgr::~InputMgr(){ // before gfxMgr destructor

}

void InputMgr::init(){

}

void InputMgr::loadLevel(){

}

void InputMgr::tick(float dt){

	keyboard->capture();
	mouse->capture();
	if(keyboard->isKeyDown(OIS::KC_ESCAPE))
		engine->stop();

//Only move the camera around if we are in gameplay
	if(engine->theState == STATE::GAMEPLAY)
	{
		UpdateCamera(dt);
	}
//	UpdateDesiredSpeedHeading(dt);
	//UpdateSelection(dt);

}

void InputMgr::stop(){
	if (oisInputManager){
		oisInputManager->destroyInputObject(mouse);
		oisInputManager->destroyInputObject(keyboard);
		OIS::InputManager::destroyInputSystem(oisInputManager);
		oisInputManager = 0;
	}
}

void InputMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void InputMgr::windowClosed(Ogre::RenderWindow* rw){
	//Ogre::WindowEventUtilities::removeWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);
	if(rw == engine->gfxMgr->ogreRenderWindow){
		engine->stop();
	}
}

bool InputMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool InputMgr::keyReleased(const OIS::KeyEvent &arg){
	std::cout << "Checking key release" << std::endl;
	if(arg.key == OIS::KC_LSHIFT)
		isSprinting = false;
	return true;
}

bool InputMgr::mouseMoved(const OIS::MouseEvent &arg){
	if(engine->theState == STATE::GAMEPLAY){

		// Rotate the camera based on the movement of the camera

		//engine->gfxMgr->cameraNode->pitch( -Ogre::Radian(arg.state.Y.rel) / 1000.f);
		//engine->gfxMgr->cameraNode->rotate(Ogre::Vector3(0,1,0), -Ogre::Radian(arg.state.X.rel) / 1000.f, Ogre::Node::TS_WORLD);

		//engine->gfxMgr->yawNode->rotate(Ogre::Vector3(0,1,0), -Ogre::Radian(arg.state.X.rel) / 1000.f, Ogre::Node::TS_WORLD);
		//engine->gfxMgr->pitchNode->rotate(Ogre::Vector3(1,0,0), -Ogre::Radian(arg.state.Y.rel) / 1000.f, Ogre::Node::TS_WORLD);

		engine->gfxMgr->yawNode->yaw(-Ogre::Radian(arg.state.X.rel) / 1000.f);
		engine->gfxMgr->pitchNode->pitch(-Ogre::Radian(arg.state.Y.rel) / 1000.f);

		//engine->gfxMgr->cameraNode->setOrientation(pitchNode->getOrientation() * yawNode->getOrientation());

	//	Ogre::Vector3 lookVector = engine->gfxMgr->ogreCamera->getCameraToViewportRay(0.5, 0.5).getDirection().normalisedCopy();
	//
	//	Ogre::Vector3 lookVectorFloor(lookVector.x, 0, lookVector.z);
	//	Ogre::Real pitchAngleSign;
	//
	//
	//
	//	pitchAngleSign = engine->gfxMgr->cameraNode->getOrientation().x;
	//
	//	Ogre::Real pitchAngle = lookVector.angleBetween(lookVectorFloor).valueDegrees();
	//
	//	std::cerr << pitchAngle << std::endl;
	//
	//	if(pitchAngle > 80.f)
	//	{
	//		if(pitchAngleSign > 0)
	//			engine->gfxMgr->cameraNode->pitch( Ogre::Radian(arg.state.Y.rel - 5) / 1000.f);
	//		else if(pitchAngleSign < 0)
	//			engine->gfxMgr->cameraNode->pitch( Ogre::Radian(arg.state.Y.rel + 5) / 1000.f);
	//	}
	}

	if(engine->uiMgr->mTrayMgr->injectMouseMove(arg)) return true;

	return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	if (id == OIS::MB_Left) {
		std::cout << "Left mouse pressed" << std::endl;
		//HandleSingleSelection();
	} else if (id == OIS::MB_Right){
		//HandleCommand();
	}
	if(engine->uiMgr->mTrayMgr->injectMouseDown(arg, id)) return true;
	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	if(engine->uiMgr->mTrayMgr->injectMouseUp(arg, id)) return true;
	return true;
}


//void InputMgr::HandleSingleSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
//void InputMgr::HandleSingleSelection(){
//	OIS::MouseState ms = mouse->getMouseState();
//	Ogre::Ray mouseRay = engine->gfxMgr->ogreCamera->getCameraToViewportRay(ms.X.abs/(float) ms.width, ms.Y.abs/(float)ms.height);
//	std::pair<bool, float> result = mouseRay.intersects(engine->gameMgr->ocean);
//	if (result.first){
//			this->posUnderMouse = mouseRay.getPoint(result.second);
//			float minDistanceSquared = FLT_MAX;
//			float distanceSquared;
//			// find ent that is 1. Within threshold distance && 2. Nearest to mouse cursor
//			for (std::list<Entity381 *>::const_iterator it = engine->entityMgr->entities.begin(); it != engine->entityMgr->entities.end(); ++it){
//				distanceSquared = this->posUnderMouse.squaredDistance((*it)->pos);
//				if (distanceSquared < selectionDistanceSquaredThreshold){
//					if (distanceSquared < minDistanceSquared){
//						engine->entityMgr->selectedEntity->isSelected = false;
//						engine->entityMgr->selectedEntity = (*it);
//						(*it)->isSelected = true;
//						minDistanceSquared = distanceSquared;
//					}
//				}
//			}
//		}
//}

//Ogre::Vector3 InputMgr::GetPositionUnderMouse(){
//	OIS::MouseState ms = mouse->getMouseState();
//	Ogre::Ray mouseRay = engine->gfxMgr->ogreCamera->getCameraToViewportRay(ms.X.abs/(float) ms.width, ms.Y.abs/(float)ms.height);
//	std::pair<bool, float> result = mouseRay.intersects(engine->gameMgr->ocean);
//	if (result.first){
//		return mouseRay.getPoint(result.second);
//	} else {
//		return Ogre::Vector3::NEGATIVE_UNIT_Y;
//	}
//}

//Entity381 * InputMgr::GetClosestEntityToPosition(Ogre::Vector3 position){
//	Entity381* closest = 0;
//	float minDistanceSquared = FLT_MAX;
//	float distanceSquared;
//	// find ent that is 1. Within threshold distance && 2. Nearest to mouse cursor
//	for (std::list<Entity381 *>::const_iterator it = engine->entityMgr->entities.begin(); it != engine->entityMgr->entities.end(); ++it){
//		distanceSquared = position.squaredDistance((*it)->pos);
//		if (distanceSquared < selectionDistanceSquaredThreshold){
//			if (distanceSquared < minDistanceSquared){
//				closest = (*it);
//				minDistanceSquared = distanceSquared;
//			}
//		}
//	}
//	return closest;
//}

//void InputMgr::HandleCommand(){
//	std::cout << "Handling command" << std::endl;
//	if (engine->entityMgr->selectedEntity != 0){
//		Ogre::Vector3 pos = GetPositionUnderMouse();
//		std::cout << "Right Click position: " << pos << std::endl;
//		if(pos != Ogre::Vector3::NEGATIVE_UNIT_Y){
//			Entity381 *closest = GetClosestEntityToPosition(pos);
//			if(closest == 0){
//				CommandMoveTo(pos);
//			} else {
//				if(keyboard->isKeyDown(OIS::KC_LCONTROL)){
//					CommandIntercept(closest);
//				} else {
//					CommandFollow(closest);
//				}
//			}
//		}
//	}
//}

//void InputMgr::AddOrSetCommand(Entity381* entity, Command *com){
//	for(std::list<Aspect*>::const_iterator ai = entity->aspects.begin(); ai != entity->aspects.end(); ++ai){
//		if( (*ai)->aspectType == ASPECT_TYPE::AI){
//			UnitAI* unitAI = dynamic_cast<UnitAI *> (*ai);
//			if (keyboard->isKeyDown(OIS::KC_LSHIFT)){
//				unitAI->AddCommand(com);
//			} else {
//				unitAI->SetCommand(com);
//			}
//			break;
//		}
//	}
//}

//void InputMgr::CommandIntercept(Entity381* target){
//	std::cout << "Intercepting: " << target->meshfile << std::endl;
//	Entity381* selectedEntity = engine->entityMgr->selectedEntity;
//	Intercept *interceptCommand = new Intercept(selectedEntity, target);
//	interceptCommand->init();
//	AddOrSetCommand(selectedEntity, interceptCommand);
//}
//
//void InputMgr::CommandFollow(Entity381 *target){
//	std::cout << "Following: " << target->meshfile << std::endl;
//	Entity381* selectedEntity = engine->entityMgr->selectedEntity;
//	Follow *followCommand = new Follow(selectedEntity, target);
//	followCommand->init();
//	AddOrSetCommand(selectedEntity, followCommand);
//}




//void InputMgr::CommandMoveTo(Ogre::Vector3 position){
//	std::cout << "Moving to: " << position << std::endl;
//	Entity381* selectedEntity = engine->entityMgr->selectedEntity;
//	MoveTo* moveToCommand = new MoveTo(selectedEntity, position);
//	moveToCommand->init();
//	AddOrSetCommand(selectedEntity, moveToCommand);
//
//}




// Game specific input handling
void InputMgr::UpdateCamera(float dt){
	float move = 50.0f;

	 if(keyboard->isKeyDown(OIS::KC_LSHIFT)){
		 isSprinting = true;
		 move *= 4;
	 } else
	 {
		 isSprinting = false;
	 }
	//Ogre::Vector3 lookVector = engine->gfxMgr->ogreCamera->getCameraToViewportRay(0.5, 0.5).getDirection().normalisedCopy();

	//std::cerr << lookVector.x << ", " << lookVector.y << ", " << lookVector.z << std::endl;

	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	if (keyboard->isKeyDown(OIS::KC_W))
		dirVec.z -= move;

	if (keyboard->isKeyDown(OIS::KC_S))
		dirVec.z += move;

	if (keyboard->isKeyDown(OIS::KC_E))
		dirVec.y += move;

	if (keyboard->isKeyDown(OIS::KC_F))
		dirVec.y -= move;

	if (keyboard->isKeyDown(OIS::KC_A))
	{
		dirVec.x -= move;
	}

	if (keyboard->isKeyDown(OIS::KC_D))
	{
		dirVec.x += move;
	}
	//engine->gfxMgr->cameraNode->setOrientation(pitchNode->getOrientation() * yawNode->getOrientation());
	engine->gfxMgr->cameraNode->translate(engine->gfxMgr->pitchNode->getOrientation() * engine->gfxMgr->yawNode->getOrientation() *dirVec *  dt, Ogre::Node::TS_LOCAL);

	Ogre::Real pitchAngle, pitchAngleSign;
	// Angle of rotation around the X-axis.
	pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(engine->gfxMgr->pitchNode->getOrientation().w)).valueDegrees());

	// Just to determine the sign of the angle we pick up above, the
	// value itself does not interest us.
	pitchAngleSign = engine->gfxMgr->pitchNode->getOrientation().x;

	// Limit the pitch between -90 degress and +90 degrees, Quake3-style.
	if (pitchAngle > 90.0f)
	{
	   if (pitchAngleSign > 0)
		   // Set orientation to 90 degrees on X-axis.
		   engine->gfxMgr->pitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
																  Ogre::Math::Sqrt(0.5f), 0, 0));
	   else if (pitchAngleSign < 0)
		   // Sets orientation to -90 degrees on X-axis.
		   engine->gfxMgr->pitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
																  -Ogre::Math::Sqrt(0.5f), 0, 0));
	}

	Ogre::Vector3 newPos = engine->gfxMgr->cameraNode->getPosition();
	newPos.y = 300;
	engine->gfxMgr->cameraNode->setPosition(newPos);

}
//
//void InputMgr::UpdateDesiredSpeedHeading(float dt){
//	keyboardTimer -= dt;
//
//	if(engine->entityMgr->selectedEntity){
//
//		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_NUMPAD8)){
//			keyboardTimer = keyTime;
//			engine->entityMgr->selectedEntity->desiredSpeed += 10;
//		}
//		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_NUMPAD2)){
//			keyboardTimer = keyTime;
//			engine->entityMgr->selectedEntity->desiredSpeed -= 10;
//		}
//		engine->entityMgr->selectedEntity->desiredSpeed =
//				std::max(engine->entityMgr->selectedEntity->minSpeed,
//						std::min(engine->entityMgr->selectedEntity->maxSpeed,
//								engine->entityMgr->selectedEntity->desiredSpeed));
//
//
//		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_NUMPAD4)){
//			keyboardTimer = keyTime;
//			engine->entityMgr->selectedEntity->desiredHeading -= 0.3f;
//		}
//		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_NUMPAD6)){
//			keyboardTimer = keyTime;
//			engine->entityMgr->selectedEntity->desiredHeading += 0.3f;
//		}
//		//entityMgr->selectedEntity->desiredHeading = FixAngle(entityMgr->selectedEntity->desiredHeading);
//	}
//
//}

//void InputMgr::UpdateSelection(float dt){
//	selectionTimer -= dt;
//	if(selectionTimer < 0 && keyboard->isKeyDown(OIS::KC_TAB)){
//		selectionTimer = this->selectionTime;
//		engine->entityMgr->SelectNextEntity();
//	}
//}




