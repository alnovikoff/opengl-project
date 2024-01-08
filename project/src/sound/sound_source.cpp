#include "sound/sound_source.h"

#include <iostream>

SoundSource::SoundSource()
{
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
	alSourcei(source, AL_LOOPING, is_loop_sound);
	alSourcei(source, AL_BUFFER, buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &source);
}

void SoundSource::update_pitch(float value)
{
	pitch = value;
	alSourcef(source, AL_PITCH, pitch);
}

void SoundSource::play(const ALuint buffer_to_play)
{
	if (buffer_to_play != buffer)
	{
		buffer = buffer_to_play;
		alSourcei(source, AL_BUFFER, (ALint)buffer);
	}

	alSourcePlay(source);
}

void SoundSource::loop(ALint state)
{
	alGetSourcei(source, AL_SOURCE_STATE, &state);
}