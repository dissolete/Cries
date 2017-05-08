/*
 * UiMgr.h
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#ifndef INC_UIMGR_H_
#define INC_UIMGR_H_

#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <mgr.h>
#include <string>
#include <fstream>

#include <SdkTrays.h>

const char WHITESPACE[] = {' ', '\t', '\n'};
const char COLON[] = {':'};

struct Token{

    char endDelim;///<Stores the char that ended the processing of this token

    std::string data;///<Stores the actual token

};




class UiMgr : public Mgr, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener, public OgreBites::SdkTrayListener
{
private:
	void loadMenu();
	std::string timeAsString(float time);
	std::string getCredits();

protected:
	virtual void windowResized(Ogre::RenderWindow *rw);
	virtual void windowClosed(Ogre::RenderWindow *rw);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button *b);
    void itemSelected(OgreBites::SelectMenu *m);

public:

	UiMgr(Engine *engine);
	~UiMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void loadGameOver(bool survived = false);
	virtual void loadHighScores(bool survived);
	virtual void stop();

	OgreBites::InputContext mInputContext;
	OgreBites::SdkTrayManager* mTrayMgr;
	Ogre::OverlaySystem* mOverlaySystem;
	OgreBites::Label *timeMonitor;
	OgreBites::Label *gameOverLabel;
	OgreBites::TextBox *highScores;
	OgreBites::Label *yourName;
	OgreBites::Label *nameLabel;
	OgreBites::Button *creditsButton;
	OgreBites::TextBox *credits;
	bool playerSurvived;
};

#endif /* INC_UIMGR_H_ */
