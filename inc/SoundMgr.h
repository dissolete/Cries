#ifndef __SOUND_MGR_INCLUDED
#define __SOUND_MGR_INCLUDED

#include <string>
#include <vector>
#include <mgr.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <wave.h>
#include <OgreVector3.h>


typedef struct
{
	ALuint source;
	std::string sourceName = "";
	Ogre::Vector3 position = Ogre::Vector3::ZERO;

} SourceInfo;

typedef struct
{
	ALuint buffer;
	std::string bufferFileName = "";
	std::string bufferName = "";
	bool looped;
	WaveInfo * wave;
} BufferInfo;

#define MAX_BUFFERS 63

class SoundMgr : public Mgr
{
public:

	SoundMgr(Engine * theEngine);
	~SoundMgr();

	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	/// Loads an audio file
	bool loadAudio(std::string audioName, std::string audioFilePath, bool looped = false);

	/// Plays an audio file by name
	void playAudio(std::string audioName, std::string sourceName, bool forceReset = true);

	/// Create new source
	bool createSource(std::string sourceName);

	/// Stops audio, duh!
	void stopSource(std::string sourceName);

	/// Sets an audio source location
	void setSourceLocation(std::string sourceName, Ogre::Vector3 newPosition);

	/// Returns whether the current state of the sound manager is good or not
	/// Used for error checking
	bool isGood() const;

	/// Returns true if the source is playing
	bool isSourcePlaying(std::string sourceName);

	/// Returns the error string generated during errors
	const std::string getErrorString() const;

	/// Stops everything
	void stopAllSources();

protected:

	/// Prints errorMessage if an OpenAL occurred. Returns true if there was an OpenAL error
	bool printError(std::string errorMessage);

	/// Magic function that assists with loading audio files. Don't worry about this function
	inline ALenum toALFormat(short channels, short samples);

	/// Tells OpenAl where our camera is positioned in the world
	void syncListenerToCamera();

	// Creates the OpenAL source that is ALWAYS located where the camera is
	bool createListenerSource();

private:

	bool m_initialized, m_errorFlag;

	std::string m_errorString;

	//////////////////////////
	// OpenAL requirements
	// ...
	std::vector<BufferInfo> m_buffers;
	std::vector<SourceInfo> m_sources;
	ALuint  m_buffersTot[MAX_BUFFERS];
	ALCdevice * m_device;
	ALCcontext * m_context;
	ALCenum m_alError;

	ALfloat m_cameraPos[3];
	ALfloat m_cameraVel[3];
	ALfloat m_cameraOrientation[6];


};

#endif
