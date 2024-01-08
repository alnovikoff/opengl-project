#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include <AL/al.h>

class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void play(const ALuint buffer_to_play);
	void loop(ALint state);

	void set_location(const float &x, const float &y, const float &z);
	void set_orientation(const float &x, const float &y, const float &z);	
	void set_velocity(const float &x, const float &y, const float &z);
	void set_gain(const float& val);
	void set_pitch(const float& val);

	void update_pitch(float value);

private:
	ALuint _source;
	float _gain = 1.0f;
	float _pitch = 1.0f;
	bool _is_loop_sound = true;
	ALuint _buffer = 0;
};


#endif // SOUND_SOURCE_H