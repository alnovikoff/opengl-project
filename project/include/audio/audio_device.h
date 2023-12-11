#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include <AL/alc.h>

class AudioDevice
{
public:
	AudioDevice();
private:

	ALCdevice* alc_device;
	ALCcontext* alc_context;
};

#endif