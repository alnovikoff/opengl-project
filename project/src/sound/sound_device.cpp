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
	_alc_device = alcOpenDevice(nullptr); // nullptr = get default device
	if (!_alc_device)
		throw("failed to get sound device");

	_alc_context = alcCreateContext(_alc_device, nullptr);  // create context
	if(!_alc_context)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(_alc_context))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(_alc_device, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(_alc_device, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(_alc_device) != AL_NO_ERROR)
		name = alcGetString(_alc_device, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");

	alcDestroyContext(_alc_context);
	if (_alc_context)
		throw("failed to unset during close");

	if (!alcCloseDevice(_alc_device))
		throw("failed to close sound device");
}