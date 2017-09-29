#include "stdafx.h"
#include "MusicPlayer.h"

#include <iostream>
#include "Windows.h"
#include "MMSystem.h"
#include <string>
#pragma comment(lib, "winmm.lib")

MusicPlayer::MusicPlayer()
{
}


MusicPlayer::~MusicPlayer()
{
	MusicPlayer::Stop();
}

void MusicPlayer::Play(std::wstring fileName) {
	PlaySound(fileName.c_str(), NULL, SND_ASYNC);
}

void MusicPlayer::Play(std::string fileName) {
	std::wstring ws;
	ws.assign(fileName.begin(), fileName.end());
	this->Play(ws);
}

void MusicPlayer::Stop() {
	PlaySound(NULL, 0, 0);
}
