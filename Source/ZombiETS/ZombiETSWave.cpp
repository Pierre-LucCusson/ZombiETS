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
}

ZombiETSWave::~ZombiETSWave()
{
	delete music;
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
