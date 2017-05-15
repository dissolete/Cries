#include <SoundMgr.h>
#include <OgreQuaternion.h>
#include <engine.h>

SoundMgr::SoundMgr(Engine * theEngine)
	: Mgr(theEngine)
	, m_initialized(false)
	, m_errorFlag(false)
	, m_errorString("")
{

}

SoundMgr::~SoundMgr()
{
	//alDeleteBuffers(NUM_BUFFERS, m_buffers);

	int ret = alcMakeContextCurrent(NULL);
	if(!ret)
	{
		this->printError("In SoundMgr::~SoundMgr(), the current context is NULL.");
	}
	// YOLO
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);

}

void SoundMgr::init()
{
	// Init openAL
	m_device = alcOpenDevice(NULL);
	if(!m_device)
	{
		m_errorFlag = true;
		this->printError("The sound device failed to be initialized by alcOpenDevice() in SoundMgr::init()");
		//m_alError = alGetError();
		//m_errorString += "* The sound device failed to be initialized by alcOpenDevice() in SoundMgr::init() with AL Error code : " + std::to_string(m_alError) +"\n";
	}

	// Create context
	m_context = alcCreateContext(m_device, NULL);
	if(!alcMakeContextCurrent(m_context) and not m_errorFlag)
	{
		m_errorFlag = true;
		this->printError("The context failed to be created by alcCreateContext() in SoundMgr::init()");
		//m_alError = alGetError();
		//m_errorString += "* The context failed to be created by alcCreateContext() in SoundMgr::init() with AL Error code : " + m_alError +"\n";
	}

	// Create buffers
	//m_buffers = new ALuint(NUM_BUFFERS);
	alGenBuffers(MAX_BUFFERS, m_buffersTot);
	if((m_alError = alGetError()) != AL_NO_ERROR)
	{
		m_errorFlag = true;
		this->printError("The buffers failed to generate in SoundMgr::init()");
		//m_errorString += "* The buffers failed to generate in SoundMgr::init() with AL Error code : " + m_alError + "\n";
	}

	m_initialized = !m_errorFlag;
	if(!m_initialized) std::cerr << "The sound manager has failed to initialize!" << std::endl;

}


void SoundMgr::tick(float dt)
{
	syncListenerToCamera();
}

void SoundMgr::loadLevel()
{

}

void SoundMgr::stop()
{

}

inline ALenum SoundMgr::toALFormat(short channels, short samples) {
	bool stereo = (channels > 1);
	switch (samples){
	case 24:
		std::cerr << "Attempt to load a 24 bit wave file. Unfortunately, this is not supported. Please convert to 16 bit quality." << std::endl;
		return -1;
	case 16:
		if (stereo) {
			return AL_FORMAT_STEREO16;
		} else {
			return AL_FORMAT_MONO16;
		}

	case 8:
		if (stereo) {
			return AL_FORMAT_STEREO8;
		} else {
			return AL_FORMAT_MONO8;
		}

	default:
		return -1;

	}
}

void SoundMgr::syncListenerToCamera()
{
	// Setting listener position
	Ogre::Vector3 cameraPos = engine->gfxMgr->cameraNode->getPosition();
	m_cameraPos[0] = cameraPos.x;
	m_cameraPos[1] = cameraPos.y;
	m_cameraPos[2] = cameraPos.z;
	alListener3f(AL_POSITION, m_cameraPos[0], m_cameraPos[1], m_cameraPos[2]);
	this->printError("Syncing the listener to the camera failed when updating AL_POSITION.");

	// Setting listener velocity
	// We are setting the camera velocity to 0 because we don't want any doppler effects calculated by openAL
	m_cameraVel[0] = 0;
	m_cameraVel[1] = 0;
	m_cameraVel[2] = 0;
	alListener3f(AL_VELOCITY, m_cameraVel[0], m_cameraVel[1], m_cameraVel[2]);
	this->printError("Syncing the listener to the camera failed when updating AL_VELOCITY.");

	// Setting listener orientation
	Ogre::Quaternion q = engine->gfxMgr->cameraNode->getOrientation();
	Ogre::Vector3 vDirection = q.zAxis();
	Ogre::Vector3 vUp = q.yAxis();
	m_cameraOrientation[0] = -vDirection.x;
	m_cameraOrientation[1] = -vDirection.y;
	m_cameraOrientation[2] = -vDirection.z;
	m_cameraOrientation[3] = vUp.x;
	m_cameraOrientation[4] = vUp.y;
	m_cameraOrientation[5] = vUp.z;
	alListenerfv(AL_ORIENTATION, m_cameraOrientation);
	this->printError("Syncing the listener to the camera failed when updating AL_ORIENTATION.");

}

bool SoundMgr::createListenerSource()
{
	alGetError();

	SourceInfo newSource;
	newSource.source = m_sources.size() + 1;
	newSource.position = engine->gfxMgr->cameraNode->getPosition();
	newSource.sourceName = "Camera";

	alGenSources(1, &newSource.source);

	alSourcef(newSource.source, AL_PITCH, 1);
	if(this->printError("Attempt to set camera source pitch failed!"))
		return false;

	alSourcef(newSource.source, AL_GAIN, 1);
	if(this->printError("Attempt to set camera source gain failed!"))
		return false;

	alSource3f(newSource.source, AL_POSITION, newSource.position.x, newSource.position.y, newSource.position.z);
	if(this->printError("Attempt to set camera source position failed!"))
		return false;

	alSource3f(newSource.source, AL_VELOCITY, 0, 0, 0);
	if(this->printError("Attempt to set camera source velocity failed!"))
		return false;

	alSourcei(newSource.source, AL_LOOPING, AL_FALSE);
	if(this->printError("Attempt to set camera source looping failed!"))
		return false;

	m_sources.push_back(newSource);

	return true;
}

bool SoundMgr::loadAudio(std::string audioName, std::string audioFilePath, bool looped)
{
	// Reset errors
	alGetError();

	BufferInfo newBuffer;

	newBuffer.bufferFileName = audioFilePath;
	newBuffer.bufferName = audioName;
	newBuffer.looped = looped;
	newBuffer.buffer = m_buffers.size() + 1;

	newBuffer.wave = WaveOpenFileForReading(audioFilePath.c_str());
	if(!newBuffer.wave)
	{
		std::cerr << "SoundMgr::loadAudio() failed! Cannot open wave file " << audioFilePath << " for reading!" << std::endl;
		return false;
	}

	int ret = WaveSeekFile(0, newBuffer.wave);
	if(ret)
	{
		std::cerr << "SoundMgr::loadAudio() failed! Cannot seek wave file " << audioFilePath <<  "!" << std::endl;
		return false;
	}

	char * tempBuff = (char*) malloc(newBuffer.wave->dataSize);
	if(!tempBuff)
	{
		std::cerr << "SoundMgr::loadAudio() failed! Error occured when attempting to malloc the wave buffer for " << audioFilePath << "!" << std::endl;
		return false;
	}
	ret = WaveReadFile(tempBuff, newBuffer.wave->dataSize, newBuffer.wave);
	if(ret != (int) newBuffer.wave->dataSize)
	{
		std::cerr << "SoundMgr::loadAudio() failed! Error occurred when reading wave file for " << audioFilePath << "! Short read " << ret << " wanted: " << newBuffer.wave->dataSize << std::endl;
		return false;
	}

//	if(!alIsBuffer(newBuffer.buffer) or newBuffer.bufferFileName == "")
//	{
//		alGenBuffers(1, &newBuffer.buffer);
//		std::string error = "Cannot generate buffer for " + audioFilePath;
//		if(printError(error)) return false;
//	}

	alBufferData(newBuffer.buffer,
			     toALFormat(newBuffer.wave->channels, newBuffer.wave->bitsPerSample),
				 tempBuff,
				 newBuffer.wave->dataSize,
				 newBuffer.wave->sampleRate
				 );

	free(tempBuff);

	if(printError("Failed to load bufferData") == true)
	{
		return false;
	}

	m_buffers.push_back(newBuffer);

	return true;
}

void SoundMgr::playAudio(std::string audioName, std::string sourceName, bool forceReset)
{
	BufferInfo bufferToPlay;
	SourceInfo sourceToUse;

	// Get buffer to play
	for(auto buffer : m_buffers)
	{
		if(buffer.bufferName == audioName)
			bufferToPlay = buffer;
	}

	// Get source to use
	for(auto source : m_sources)
	{
		if(source.sourceName == sourceName)
			sourceToUse = source;
	}

	// Check if these were found
	if(bufferToPlay.bufferName == "")
	{
		std::cerr << "Could not play " << audioName << " because the buffer could not be found!" << std::endl;
		return;
	}
	if(sourceToUse.sourceName == "")
	{
		std::cerr << "Could not play " << audioName << " because the source " << sourceName << " requested could not be found!" << std::endl;
		return;
	}

	// Lets tell OpenAL to bind the buffer to the source
	alSourcei(sourceToUse.source, AL_BUFFER, bufferToPlay.buffer);
	if(printError("Failed to bind source to buffer in playAudio()"))
	{
		return;
	}

	alSourcePlay(sourceToUse.source);
	if(printError("Failed to play audio source in playAudio()"))
	{
		return;
	}


}

bool SoundMgr::createSource(std::string sourceName)
{
	alGetError();

	SourceInfo newSource;
	newSource.source = m_sources.size() + 1;
	newSource.sourceName = sourceName;

	alGenSources(1, &newSource.source);

	alSourcef(newSource.source, AL_PITCH, 1);
	if(this->printError("Attempt to set source pitch failed!"))
		return false;

	alSourcef(newSource.source, AL_GAIN, 1);
	if(this->printError("Attempt to set source gain failed!"))
		return false;

	alSource3f(newSource.source, AL_POSITION, newSource.position.x, newSource.position.y, newSource.position.z);
	if(this->printError("Attempt to set source position failed!"))
		return false;

	alSource3f(newSource.source, AL_VELOCITY, 0, 0, 0);
	if(this->printError("Attempt to set source velocity failed!"))
		return false;

	alSourcei(newSource.source, AL_LOOPING, AL_FALSE);
	if(this->printError("Attempt to set source looping failed!"))
		return false;

	m_sources.push_back(newSource);

	return true;
}

void SoundMgr::stopSource(std::string sourceName)
{
	SourceInfo sourceToStop;

	// Lets find this buffer
	for(auto source : m_sources)
	{
		if(source.sourceName == sourceName)
			sourceToStop = source;
	}

	// Check if it was found
	if(sourceToStop.sourceName  == "")
	{
		std::cerr << "Failed to stop " << sourceName << " because it could not be found in the source list!" << std::endl;
		return;
	}

	alSourceStop(sourceToStop.source);
	printError("Failed to stop source " + sourceName);

}

void SoundMgr::setSourceLocation(std::string sourceName, Ogre::Vector3 newPosition)
{
	for(auto source : this->m_sources)
	{
		// We're assuming that every source name is unique
		if(source.sourceName == sourceName){
			source.position = newPosition;
			return;
		}
	}

	std::cerr << "The source " << sourceName << " was not found in the source list so the position was not set!" << std::endl;

}

bool SoundMgr::isGood() const
{
	return !m_errorFlag;
}

bool SoundMgr::isSourcePlaying(std::string sourceName)
{
	SourceInfo theSource;

	// Lookup source
	for(auto source : m_sources)
	{
		if(source.sourceName == sourceName)
			theSource = source;
	}

	// Check if found
	if(theSource.sourceName == "")
	{
		std::cerr << "Failed to locate source " << sourceName << " when searching in the source list during isSourcePlaying()." << std::endl;
		return false;
	}

	ALint sourceState;
	alGetSourcei(theSource.source, AL_SOURCE_STATE, &sourceState);
	if(printError("Failed to locate source " + sourceName + " when checking if it was playing."))
	{
		return false;
	}

	return sourceState == AL_PLAYING;
}

const std::string SoundMgr::getErrorString() const
{
	return m_errorString;
}

bool SoundMgr::printError(std::string errorMessage)
{
	if((m_alError = alGetError()) != AL_NO_ERROR)
	{
		std::cerr << "Error in SoundMgr: " << errorMessage << " [AL ERROR: " << m_alError << "]" << std::endl;
		return true;
	}

	return false;
}

void SoundMgr::stopAllSources()
{
	for(auto source : m_sources)
	{
		alSourceStop(source.source);
	}
}
