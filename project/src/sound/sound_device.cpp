#include "sound/sound_device.h"

#include <AL\al.h>
#include <stdio.h>

SoundDevice* SoundDevice::get()
{
	static SoundDevice* snd_device = new SoundDevice();
	return snd_device;
}

SoundDevice::SoundDevice()
{
	alc_device = alcOpenDevice(nullptr); // nullptr = get default device
	if (!alc_device)
		throw("failed to get sound device");

	alc_context = alcCreateContext(alc_device, nullptr);  // create context
	if(!alc_context)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(alc_context))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(alc_device, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(alc_device, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(alc_device) != AL_NO_ERROR)
		name = alcGetString(alc_device, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");

	alcDestroyContext(alc_context);
	if (alc_context)
		throw("failed to unset during close");

	if (!alcCloseDevice(alc_device))
		throw("failed to close sound device");
}