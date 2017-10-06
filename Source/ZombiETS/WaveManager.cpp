// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "WaveManager.h"

WaveManager::WaveManager()
{
	music = new ZombiETSMusicPlayer();
	files = new FilesInDirectory(".wav");
	files->ReadDirectory(FilesInDirectory::GetMyDocuments() + TEXT("\\ZombiETS\\music"));

	waves = new std::vector<ZombiETSWave*>();
}

WaveManager::~WaveManager()
{
	delete music;
	delete files;
	delete waves;
}

void WaveManager::StartNextWave()
{
	if (waveCount > 0)
		CurrentWave()->Stop();
	AddNewWave();
	CurrentWave()->Start();
}

void WaveManager::StopWave()
{
	ZombiETSWave* wave = CurrentWave();
	if (wave != nullptr)
		wave->Stop();
}

ZombiETSWave* WaveManager::AddNewWave()
{
	waveCount++;
	ZombiETSWave* wave = new ZombiETSWave(files->GetRandomFile(), waveCount);
	waves->push_back(wave);
	return wave;
}

ZombiETSWave * WaveManager::CurrentWave()
{
	if (waveCount == 0)
		return nullptr;
	return waves->at(waveCount-1);
}
