/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 *      Editied: Hadi Rumjahn, Gage Thomas, Jake Shepard
 */

#include <UiMgr.h>
#include <engine.h>

template <typename StorageType>
bool contains(const StorageType item, const StorageType data[], const int len) {

    //Iterates through all values in the array
    for(int index = 0; index < len; index++){

        //If the data was found, return true
        if(data[index] == item) {

            return true;

        }

    }

    //Item was not found
    return false;

}

Token getMyToken(std::ifstream &fin, const char delims[], const int len){

    char character;

    bool leadWS = true, foundDelim = false;;

    Token result;

    //Initialize the token result
    result.data = "";

    //Loops until the file is empty or a deliminator is reached (after leading
    //whitespace is eliminated)
    while(!fin.eof() && !foundDelim) {

        fin.get(character);


        //If the character was not whitespace, set the flag
        if(leadWS && !(character == ' ' || character == '\t' || character == '\n')) {

            leadWS = false;

        }

        //If not processing leading whitespace
        if(!leadWS){

            //If a deliminator is reached after leading whitespace, end loop
            if(contains(character, delims, len)) {

                result.endDelim = character;
                foundDelim = true;

            } else {

                result.data += character;//Else, append the current char

            }
        }

    }

    //If a deliminator was not found, return an empty string
    if(!foundDelim) {

        Token failResult;
        failResult.data = "";
        return failResult;

    } else {//Else return the token

        return result;
    }

}

UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	    mOverlaySystem = new Ogre::OverlaySystem();
	    engine->gfxMgr->ogreSceneManager->addRenderQueueListener(mOverlaySystem);
	    mTrayMgr = 0;
	    timeMonitor = 0;
	    gameOverLabel = 0;
	    highScores = 0;
	    nameLabel = 0;
	    yourName = 0;//Not a rating for the movie, that movie was good
	    creditsButton = 0;
	    credits = 0;
	    playerSurvived = false;

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

void UiMgr::loadGameOver(bool survived)
{
	playerSurvived = survived;
	int width = engine->gfxMgr->ogreRenderWindow->getWidth();
	int height = engine->gfxMgr->ogreRenderWindow->getHeight();

	if(!survived) {
		gameOverLabel = mTrayMgr->createLabel(OgreBites::TL_NONE, "GameOverLabel", "You Died!");
	} else
	{
		gameOverLabel = mTrayMgr->createLabel(OgreBites::TL_NONE, "GameOverLabel", "You Survived!");
	}
	gameOverLabel->getOverlayElement()->setPosition((width / 2) - (gameOverLabel->getOverlayElement()->getWidth() / 2), 300);
	yourName = mTrayMgr->createLabel(OgreBites::TL_NONE, "YourName", "Your name:");
	yourName->getOverlayElement()->setPosition((width / 2) - (yourName->getOverlayElement()->getWidth() / 2), 340);
	mTrayMgr->showCursor();
	nameLabel = mTrayMgr->createLabel(OgreBites::TL_NONE, "NameLabel", "");
	nameLabel->getOverlayElement()->setPosition((width / 2) - (nameLabel->getOverlayElement()->getWidth() / 2), 380);

	//Handle the credits
	creditsButton = mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "CreditsButton", "Credits");
	credits = mTrayMgr->createTextBox(OgreBites::TL_NONE, "Credits", "Credits:", 300, 600);
	credits->getOverlayElement()->setPosition(width - 300, height - 50 - 600);
	credits->setText(getCredits());
	//Do not center it, it alligns the left of the string with the center
	credits->hide(); //For some reason if this call is made now, the text gets cleared
	credits->setText(getCredits());

}

std::string UiMgr::getCredits()
{
	return "Cries\n\nPresented by Code R3d\n\nGage Thomas\n\nJake Shepherd\n\nHadi Rumjahn\n";
}

void UiMgr::loadHighScores(bool survived)
{
	std::string names[10];
	float scores[10];
	std::ifstream fin;
	std::string filename;
	int width = engine->gfxMgr->ogreRenderWindow->getWidth();
	bool playerUsed = false;
	if(survived)
	{
		filename = "HighScoresSurvived.txt";
	} else
	{
		filename = "HighScoresDied.txt";
	}
	fin.open(filename);

	//Enter all the information into the two arrays
	for(int index = 0; index < 10; index++)
	{
		Token nextToken = getMyToken(fin, COLON, 1);//gets the name
		Token scoreToken = getMyToken(fin, WHITESPACE, 3);
		float nextScore = stof(scoreToken.data);

		//If the player beat the score
		if((survived && engine->gameMgr->gameplayTime < nextScore && !playerUsed) ||
				(!survived && engine->gameMgr->gameplayTime > nextScore && !playerUsed))
		{
			playerUsed = true;
			if(nameLabel->getCaption().length() == 0)
			{
				names[index] = "Kira";
			} else
			{
				names[index] = nameLabel->getCaption();
			}
			scores[index] = engine->gameMgr->gameplayTime;
			index++;
		}
		//Now handle the current score, so long as the player's score didn't push
		//index past 10
		if(index < 10)
		{
			names[index] = nextToken.data;
			scores[index] = nextScore;
		}
	}
	fin.close();
	std::ofstream fout;
	fout.open(filename);

	//Now write the new high scores to file
	for(int index = 0; index < 10; index++)
	{
		fout << names[index] << ": " << scores[index] << std::endl;
	}
	fout << std::endl;
	fout.close();

	//Now display the high scores
	yourName->hide();
	nameLabel->hide();
	std::string scoresText = "";
	for(int index = 0; index < 10; index++)
	{
		scoresText += std::to_string(index + 1) + ". " + names[index] + ": " + timeAsString(scores[index]) + "\n\n";
	}
	highScores = mTrayMgr->createTextBox(OgreBites::TL_NONE, "HighScores", "High Scores:", 300, 600);
	highScores->getOverlayElement()->setPosition((width / 2) - (highScores->getOverlayElement()->getWidth() / 2), 340);

	highScores->setText(scoresText);
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
	} else if(engine->theState == STATE::GAMEPLAY)
	{
		timeMonitor->setCaption(timeAsString(engine->gameMgr->gameplayTime));
	} else if(engine->theState == STATE::GAMEOVER)
	{

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
    } else if(b->getName() == "CreditsButton")
    {
    	if(credits->isVisible())
    	{
    		credits->hide();
    	} else
    	{
    		credits->setText(getCredits());
    		credits->show();
    	}
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

	// LOAD MAIN MENU SOUND
	//engine->soundMgr->load_song("Menu", "resources/Cries - Theme.ogg");
	engine->soundMgr->load_song("Menu", "sounds/Cries-Theme.wav");
	//load_sound(std::string soundName, std::string filePath);

	//play_sound(std::string soundName);
	engine->soundMgr->play_song2D("Menu", true);

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
