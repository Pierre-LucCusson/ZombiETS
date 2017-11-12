// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "ZombiETSWave.h"

#include <fstream>

void ZombiETSWave::SetNameFromPath()
{
	std::istringstream f(musicPath);
	std::string s;
	while (getline(f, s, '\\')) {
		musicName = FString(s.c_str());
	}
}

ZombiETSWave::ZombiETSWave(std::string musicPath, int number)
{
	this->musicPath = musicPath;
	this->number = number;
	SetNameFromPath();
	music = new ZombiETSMusicPlayer();

	musicData = FindBeats(musicPath.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Wave: Music analysis complete."));
}

ZombiETSWave::~ZombiETSWave()
{
	delete music;
	delete musicData;
}

int ZombiETSWave::GetNumber()
{
	return number;
}

FString ZombiETSWave::GetName()
{
	return musicName;
}

void ZombiETSWave::Start()
{
	music->Play(musicPath);
}

void ZombiETSWave::Stop()
{
	music->Stop();
}
