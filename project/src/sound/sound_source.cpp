#include "sound/sound_source.h"

#include <iostream>

SoundSource::SoundSource()
{
	alGenSources(1, &_source);
	alSourcef(_source, AL_PITCH, _pitch);
	alSourcef(_source, AL_GAIN, _gain);
	alSourcei(_source, AL_LOOPING, _is_loop_sound);
	alSourcei(_source, AL_BUFFER, _buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &_source);
}

void SoundSource::update_pitch(float value)
{
	_pitch = value;
	alSourcef(_source, AL_PITCH, _pitch);
}

void SoundSource::play(const ALuint buffer_to_play)
{
	if (buffer_to_play != _buffer)
	{
		_buffer = buffer_to_play;
		alSourcei(_source, AL_BUFFER, (ALint)_buffer);
	}

	alSourcePlay(_source);
}

void SoundSource::loop(ALint state)
{
	alGetSourcei(_source, AL_SOURCE_STATE, &state);
}