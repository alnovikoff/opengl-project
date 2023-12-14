#ifndef SOUND_BUFFER_H
#define SOUND_BUFFER_H

#include <AL\al.h>
#include <vector>

class SoundBuffer
{
public:
	static SoundBuffer* get();

	ALuint add_sound_effect(const char* filename);
	bool remove_sound_effect(const ALuint& buffer);

private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> _sound_effect_buffers;
};


#endif