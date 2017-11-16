// Fill out your copyright notice in the Description page of Project Settings.
#include "Windows.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <istream>
#include <chrono>

#include "ZombiETSMusicPlayer.h"
#include "BeatFinder.h"

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
	float maxSpectralFlux;
	int maxSpectralPeak;

	std::chrono::system_clock::time_point atStart;
	std::chrono::system_clock::time_point atEnd;

	void SetNameFromPath();
public:
	ZombiETSWave(std::string musicPath, int number = 0);
	~ZombiETSWave();

	int GetNumber();
	FString GetName();
	void Start();
	void Stop();

	long Time();
	float GetInstantSpectralFlux();
	float GetMaximumSpectralFlux();
	float GetAverageSpectralFlux(int interval);
	int GetInstantSpectralPeak();
	int GetMaximumSpectralPeak();
	float GetAverageSpectralPeak(int interval);
};
