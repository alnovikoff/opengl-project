#ifndef SOUND_DEVICE_H
#define SOUND_DEVICE_H

#include <AL/alc.h>

class SoundDevice
{
public:
	static SoundDevice* get();

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* _alc_device;
	ALCcontext* _alc_context;

};

#endif