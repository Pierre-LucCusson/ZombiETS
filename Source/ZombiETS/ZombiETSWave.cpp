// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "ZombiETSWave.h"

#include <fstream>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <numeric>

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

	std::wstring widePath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(musicPath);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Analyzing: %s"), widePath.c_str()));

	musicData = FindBeats(musicPath.c_str());

	this->maxSpectralFlux = *std::max_element(musicData->spectralFlux.begin(), musicData->spectralFlux.end());
	this->maxSpectralPeak = *std::max_element(musicData->peaks.begin(), musicData->peaks.end());

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Maximum spectral flux value: %f"), maxFlux));
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
	atStart = std::chrono::system_clock::now();
}

void ZombiETSWave::Stop()
{
	music->Stop();
	atEnd = std::chrono::system_clock::now();
}

long ZombiETSWave::Time()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	if (atEnd != std::chrono::system_clock::time_point())
	{
		now = atEnd;
	}

	return std::chrono::duration_cast<std::chrono::milliseconds>(now - atStart).count();
}

float ZombiETSWave::GetInstantSpectralFlux()
{
	long currentTime = this->Time();
	int sampleRate = 44100;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	return musicData->spectralFlux.at(round(currentTime * windowRate));
}

float ZombiETSWave::GetAverageSpectralFlux(int interval)
{
	double halfInterval = interval / 2.0;
	long currentTime = this->Time();
	int sampleRate = 44100;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	
	long startTime = currentTime - halfInterval;
	long endTime = currentTime + halfInterval;

	std::vector<float> sub(musicData->spectralFlux.begin() + max(0, round(startTime * windowRate)), musicData->spectralFlux.begin() + min(musicData->spectralFlux.size() - 1, round(endTime * windowRate)));

	return 1.0 * std::accumulate(sub.begin(), sub.end(), 0LL) / sub.size();
}

float ZombiETSWave::GetMaximumSpectralFlux()
{
	return maxSpectralFlux;
}

int ZombiETSWave::GetInstantSpectralPeak()
{
	long currentTime = this->Time();
	int sampleRate = 44100;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	return musicData->peaks.at(round(currentTime * windowRate));
}

float ZombiETSWave::GetAverageSpectralPeak(int interval)
{
	double halfInterval = interval / 2.0;
	long currentTime = this->Time();
	int sampleRate = 44100;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond

	long startTime = currentTime - halfInterval;
	long endTime = currentTime + halfInterval;

	std::vector<int> sub(musicData->peaks.begin() + max(0, round(startTime * windowRate)), musicData->peaks.begin() + min(musicData->spectralFlux.size() - 1, round(endTime * windowRate)));

	return 1.0 * std::accumulate(sub.begin(), sub.end(), 0LL) / sub.size();
}

int ZombiETSWave::GetMaximumSpectralPeak()
{
	return maxSpectralPeak;
}
