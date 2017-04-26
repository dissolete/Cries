/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#include <UiMgr.h>
#include <engine.h>

UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	    mOverlaySystem = new Ogre::OverlaySystem();
	    engine->gfxMgr->ogreSceneManager->addRenderQueueListener(mOverlaySystem);

	    //Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);
}

UiMgr::~UiMgr(){ // before gfxMgr destructor

}

void UiMgr::init(){
	//init sdktrays

    mInputContext.mKeyboard = engine->inputMgr->keyboard;
    mInputContext.mMouse = engine->inputMgr->mouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->ogreRenderWindow, mInputContext, this);
    //mTrayMgr->hideCursor();
}

void UiMgr::stop(){

}

void UiMgr::loadLevel(){
	mTrayMgr->hideCursor();
}

void UiMgr::tick(float dt){
	mTrayMgr->refreshCursor();
	//Update the time since last event if in splash screen
	if(engine->theState == STATE::SPLASH)
	{
		engine->timeSinceLastEvent += dt;
		//If 3 seconds have passed, go into gameplay
		if(engine->timeSinceLastEvent >= 3)
		{
			engine->theState = STATE::MAIN_MENU;
			engine->gfxMgr->loadMenu();
			loadMenu();//Creates the button
		}
	}
}

void UiMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = engine->inputMgr->mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void UiMgr::windowClosed(Ogre::RenderWindow* rw){

}

bool UiMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool UiMgr::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
bool UiMgr::mouseMoved(const OIS::MouseEvent &arg){
    if (mTrayMgr->injectMouseMove(arg)) return true;
    /* normal mouse processing here... */
	return true;
}
bool UiMgr::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	std::cout << "mouse clicked" << std::endl;
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
	    /* normal mouse processing here... */
	return true;
}
bool UiMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    /* normal mouse processing here... */
	return true;
}

void UiMgr::buttonHit(OgreBites::Button *b){
    if(b->getName()=="NewGame")
    {
        std::cout <<"New Game pressed" << std::endl;
        engine->theState = STATE::GAMEPLAY;
        engine->loadLevel();
        mTrayMgr->destroyWidget(b);
    }

}

void UiMgr::itemSelected(OgreBites::SelectMenu *m){
    if(m->getName()=="MyMenu")
    {
        std::cout <<"Menu!" << std::endl;
    }

}

void UiMgr::loadMenu()
{
	mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "NewGame", "New Game");
}
