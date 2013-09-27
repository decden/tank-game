#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include <string>
#include <map>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	playSound()

private:
	std::map<std::string, unsigned char*> audio_files;
};

#endif // __SOUND_MANAGER_H__