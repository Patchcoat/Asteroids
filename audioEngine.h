#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include "fmod.hpp"
#include "fmod_studio.hpp"
//#include <fmod.h>
//#include <fmod_studio.h>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

struct Vector3
{
  float x;
  float y;
  float z;
};

struct Implementation {
  Implementation();
  ~Implementation();

  void advance();

  FMOD::Studio::System* mpStudioSystem;
  FMOD::System* mpSystem;

  int mnNextChannelId;

  typedef map<string, FMOD::Sound*> SoundMap;
  typedef map<int, FMOD::Channel*> ChannelMap;
  typedef map<string, FMOD::Studio::EventInstance*> EventMap;
  typedef map<string, FMOD::Studio::Bank*> BankMap;

  BankMap mBanks;
  EventMap mEvents;
  SoundMap mSounds;
  ChannelMap mChannels;
};

class AudioEngine {
public:
    static void Init();
    static void advance();
    static void Shutdown();
    static int ErrorCheck(FMOD_RESULT result);

    void LoadSound(const string& strSoundName, bool b3d = false, bool bLooping = false, bool bStream = false);
    void UnLoadSound(const string& strSoundName);
    void Set3dListenerAndOrientation(const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
    int PlaySound(const string& strSoundName, const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
    void StopChannel(int nChannelId);
    void StopAllChannels();
    void SetChannel3dPosition(int nChannelId, const Vector3& vPosition);
    void SetChannelVolume(int nChannelId, float fVolumedB);
    FMOD::DSP * LowPassOnChannel(int nChannelId, float cutoffFreq);
    void SetChannelMute(int nChannelId, bool bMute);
    void RemoveDSP(int nChannelId, FMOD::DSP *dsp);
    bool IsPlaying(int nChannelId) const;
    float dBToVolume(float db);
    float VolumeTodB(float volume);
    FMOD_VECTOR VectorToFmod(const Vector3& vPosition);
};

#endif
