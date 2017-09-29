#include <string>
using namespace std;
#pragma once
class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();

	void Play(std::wstring fileName);
	void Play(std::string fileName);
	void Stop();
};

