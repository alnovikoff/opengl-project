#include "sound/sound_source.h"

#include <iostream>

SoundSource::SoundSource()
{
	alGenSources(1, &_source);
	alSourcef(_source, AL_PITCH, _pitch);
	alSourcef(_source, AL_GAIN, _gain);
	//alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(_source, AL_VELOCITY, _velocity[0], _velocity[1], _velocity[2]);
	alSourcei(_source, AL_LOOPING, _is_loop_sound);
	alSourcei(_source, AL_BUFFER, _buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &_source);
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


void SoundSource::set_location(const float &x, const float &y, const float &z)
{
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
	alSource3f(_source, AL_POSITION, _position[0], _position[1], _position[2]);
}
// void SoundSource::set_orientation(const float &x, const float &y, const float &z)
// {

// }
// void SoundSource::set_velocity(const float &x, const float &y, const float &z);
// void SoundSource::set_gain(const float& val);
// void SoundSource::set_pitch(const float& val);