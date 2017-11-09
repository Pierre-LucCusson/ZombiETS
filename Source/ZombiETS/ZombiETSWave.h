// Fill out your copyright notice in the Description page of Project Settings.
#include "Windows.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <istream>

#include "ZombiETSMusicPlayer.h"

#pragma once

/**
 * 
 */
class ZOMBIETS_API ZombiETSWave
{
private:
	int number;
	std::string musicPath;
	FString musicName;
	ZombiETSMusicPlayer* music;
	BeatInfo* musicData;

	void SetNameFromPath();
public:
	ZombiETSWave(std::string musicPath, int number = 0);
	~ZombiETSWave();

	int GetNumber();
	FString GetName();
	void Start();
	void Stop();
};
