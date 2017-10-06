// Fill out your copyright notice in the Description page of Project Settings.
#include "FilesInDirectory.h"
#include "ZombiETSWave.h"

#pragma once

/**
 * 
 */
class ZOMBIETS_API WaveManager
{
private:
	FilesInDirectory* files;
	ZombiETSMusicPlayer* music;

	std::vector<ZombiETSWave*>* waves;
	int waveCount = 0;
public:
	WaveManager();
	~WaveManager();

	void StartNextWave();
	void StopWave();

	ZombiETSWave* AddNewWave();
	ZombiETSWave* CurrentWave();
};
