/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 *      Editied: Hadi Rumjahn, Gage Thomas, Jake Shepard
 */

#include <UiMgr.h>
#include <engine.h>

UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	    mOverlaySystem = new Ogre::OverlaySystem();
	    engine->gfxMgr->ogreSceneManager->addRenderQueueListener(mOverlaySystem);
	    mTrayMgr = 0;
	    timeMonitor = 0;

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

void UiMgr::stop()
{

}

void UiMgr::loadLevel(){
	mTrayMgr->hideCursor();
	timeMonitor = mTrayMgr->createLabel(OgreBites::TL_TOP, "Timer", timeAsString(engine->gameMgr->gameplayTime));
	//timeMonitor->getOverlayElement()->setMaterialName("TransparentBackground.png");//Not working for some reason
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

			// LOAD MAIN MENU SOUND
			engine->soundMgr->load_song("Layer 1", "/home/hrumjahn/workspace/Cries/resources/theme.wav");
			//load_sound(std::string soundName, std::string filePath);

			//play_sound(std::string soundName);
			engine->soundMgr->play_song("Layer 1", true);

			loadMenu();//Creates the button
		}
	} else if(engine->theState == STATE::GAMEPLAY)
	{
		timeMonitor->setCaption(timeAsString(engine->gameMgr->gameplayTime));
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

std::string UiMgr::timeAsString(float time)
{
	std::string result = "";
	int resultTime = (int) time;
	int seconds = resultTime % 60;
	int minutes = resultTime / 60;
	if(minutes < 10)
	{
		result += "0";
	}
	result += std::to_string(minutes) + ":";
	if(seconds < 10)
	{
		result += "0";
	}
	result += std::to_string(seconds);

	return result;
}
