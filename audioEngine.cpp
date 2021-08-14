#include "audioEngine.h"

using namespace std;
// Initilize FMOD
// This lets us play sounds
Implementation::Implementation()
{
  mpStudioSystem = NULL;
  AudioEngine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
  AudioEngine::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

  mpSystem = NULL;
  AudioEngine::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}

// Clean up FMOD
// unload and release all the assets
Implementation::~Implementation()
{
  AudioEngine::ErrorCheck(mpStudioSystem->unloadAll());
  AudioEngine::ErrorCheck(mpStudioSystem->release());
}

void Implementation::advance()
{
  vector<ChannelMap::iterator> pStoppedChannels;
  for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
  {
    bool bIsPlaying = false;
    it->second->isPlaying(&bIsPlaying);
    if (!bIsPlaying)
    {
      pStoppedChannels.push_back(it);
    }
  }
  for (auto& it : pStoppedChannels)
  {
    mChannels.erase(it);
  }
  AudioEngine::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

// Initalize and set up an advance function for the audio
// engine
void AudioEngine::Init()
{
  sgpImplementation = new Implementation;
}

void AudioEngine::advance()
{
  sgpImplementation->advance();
}

// Load a sound
// strSoundName: Filename of the sound
// b3d: is this a 3d sound?
// bLooping: is this a looping sound?
// bStream: is the sound being streamed or use as a sample?
void AudioEngine::LoadSound(const string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
  auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
  if (tFoundIt != sgpImplementation->mSounds.end())
    return;

  FMOD_MODE eMode = FMOD_DEFAULT;
  eMode |= b3d ? FMOD_3D : FMOD_2D;
  eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
  eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

  FMOD::Sound* pSound = nullptr;
  AudioEngine::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
  if (pSound)
  {
    sgpImplementation->mSounds[strSoundName] = pSound;
  }
}

// Unload a sound that is no longer needed to free up memory
void AudioEngine::UnLoadSound(const std::string& strSoundName)
{
  auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
  if (tFoundIt == sgpImplementation->mSounds.end())
    return;

  AudioEngine::ErrorCheck(tFoundIt->second->release());
  sgpImplementation->mSounds.erase(tFoundIt);
}

// Play a sound.
// takes in the sound name, the 3d position of the sound and the volume in decibels
int AudioEngine::PlaySound(const string& strSoundName, const Vector3& vPos, float fVolumedB)
{
    int nChannelId = sgpImplementation->mnNextChannelId++;
    auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
    if (tFoundIt == sgpImplementation->mSounds.end())
    {
      LoadSound(strSoundName);
      tFoundIt = sgpImplementation->mSounds.find(strSoundName);
      if (tFoundIt == sgpImplementation->mSounds.end())
      {
        return nChannelId;
      }
    }
    FMOD::Channel* pChannel = nullptr;
    AudioEngine::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
    if (pChannel)
    {
      FMOD_MODE currMode;
      tFoundIt->second->getMode(&currMode);
      if (currMode & FMOD_3D)
      {
        FMOD_VECTOR position = VectorToFmod(vPos);
        AudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
      }
      AudioEngine::ErrorCheck(pChannel->setVolume(dBToVolume(fVolumedB)));
      AudioEngine::ErrorCheck(pChannel->setPaused(false));
      sgpImplementation->mChannels[nChannelId] = pChannel;
    }
    return nChannelId;
}

// set the 3d position of a sound channel
void AudioEngine::SetChannel3dPosition(int nChannelId, const Vector3& vPosition)
{
  auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
  if (tFoundIt == sgpImplementation->mChannels.end())
    return;

  FMOD_VECTOR position = VectorToFmod(vPosition);
  AudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

// set the volume of a channel
void AudioEngine::SetChannelVolume(int nChannelId, float fVolumedB)
{
  auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
  if (tFoundIt == sgpImplementation->mChannels.end())
    return;

  AudioEngine::ErrorCheck(tFoundIt->second->setVolume(dBToVolume(fVolumedB)));
}

// mute or unmute a channel
void AudioEngine::SetChannelMute(int nChannelId, bool bMute)
{
  auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
  if (tFoundIt == sgpImplementation->mChannels.end())
    return;

  AudioEngine::ErrorCheck(tFoundIt->second->setMute(bMute));
}

// apply a low pass filter to a channel
FMOD::DSP * AudioEngine::LowPassOnChannel(int nChannelId, float cutoffFreq)
{
  auto channel = sgpImplementation->mChannels.find(nChannelId);
  FMOD::DSP *dsp_LowPass = nullptr;
  if (channel == sgpImplementation->mChannels.end())
    return dsp_LowPass;

  AudioEngine::ErrorCheck(sgpImplementation->mpSystem->createDSPByType(FMOD_DSP_TYPE_MULTIBAND_EQ, &dsp_LowPass));
  AudioEngine::ErrorCheck(dsp_LowPass->setParameterInt(FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_12DB));
  AudioEngine::ErrorCheck(dsp_LowPass->setParameterFloat(FMOD_DSP_MULTIBAND_EQ_A_FREQUENCY, cutoffFreq));
  AudioEngine::ErrorCheck(channel->second->addDSP(0, dsp_LowPass));

  return dsp_LowPass;
}

// convert between our Vector and the Fmod vector
FMOD_VECTOR AudioEngine::VectorToFmod(const Vector3& vPosition){
  FMOD_VECTOR fVec;
  fVec.x = vPosition.x;
  fVec.y = vPosition.y;
  fVec.z = vPosition.z;
  return fVec;
}

// functions for converting between percent volume and dB and visa versa
float AudioEngine::dBToVolume(float dB)
{
  return powf(10.0f, 0.05f * dB);
}

float AudioEngine::VolumeTodB(float volume)
{
  return 20.0f * log10f(volume);
}

// error checking for FMOD
int AudioEngine::ErrorCheck(FMOD_RESULT result) {
  if (result != FMOD_OK)
  {
    cout << "FMOD ERROR " << result << endl;
    return 1;
  }
  // cout << "FMOD all good" << endl;
  return 0;
}

// shutdown the audio engine implementation
void AudioEngine::Shutdown() {
  delete sgpImplementation;
}
