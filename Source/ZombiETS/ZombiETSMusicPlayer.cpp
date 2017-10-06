// Fill out your copyright notice in the Description page of Project Settings.
#include "ZombiETS.h"
#include "ZombiETSMusicPlayer.h"
#include <string>

#pragma comment(lib, "winmm.lib")


ZombiETSMusicPlayer::ZombiETSMusicPlayer()
{
}

ZombiETSMusicPlayer::~ZombiETSMusicPlayer()
{
	this->Stop();
}

void ZombiETSMusicPlayer::Play(FString filename)
{
	std::string s = std::string(TCHAR_TO_UTF8(*filename));
	this->Play(s);
}

void ZombiETSMusicPlayer::Play(std::string filename)
{
	std::wstring ws;
	ws.assign(filename.begin(), filename.end());
	this->Play(ws);
}

void ZombiETSMusicPlayer::Play(std::wstring filename)
{
	PlaySound(filename.c_str(), 0, SND_ASYNC);
}

void ZombiETSMusicPlayer::Stop()
{
	PlaySound(NULL, 0, 0);
}
