// Fill out your copyright notice in the Description page of Project Settings.

#define _USE_MATH_DEFINES

#include "ZombiETS.h"
#include "ZombiETSWave.h"

#include <fstream>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <numeric>
#include <math.h>

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

	this->maxAmplitude = *std::max_element(musicData->amplitudeEnvelope.begin(), musicData->amplitudeEnvelope.end());
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

float ZombiETSWave::GetInstantAmplitude()
{
	long currentTime = this->Time();
	int sampleRate = musicData->sampleRate;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	return musicData->amplitudeEnvelope.at(round(currentTime * windowRate));
}

float ZombiETSWave::GetAverageAmplitude(int interval)
{
	double halfInterval = interval / 2.0;
	long currentTime = this->Time();
	int sampleRate = musicData->sampleRate;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond

	long startTime = currentTime - halfInterval;
	long endTime = currentTime + halfInterval;

	std::vector<float> sub(musicData->amplitudeEnvelope.begin() + max(0, round(startTime * windowRate)), musicData->amplitudeEnvelope.begin() + min(musicData->amplitudeEnvelope.size() - 1, round(endTime * windowRate)));

	return 1.0 * std::accumulate(sub.begin(), sub.end(), 0.0f) / sub.size();
}

float ZombiETSWave::GetMaximumAmplitude()
{
	return maxAmplitude;
}

float ZombiETSWave::GetInstantSpectralFlux()
{
	long currentTime = this->Time();
	int sampleRate = musicData->sampleRate;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	return musicData->spectralFlux.at(round(currentTime * windowRate));
}

float ZombiETSWave::GetAverageSpectralFlux(int interval)
{
	double halfInterval = interval / 2.0;
	long currentTime = this->Time();
	int sampleRate = musicData->sampleRate;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	
	long startTime = currentTime - halfInterval;
	long endTime = currentTime + halfInterval;

	std::vector<float> sub(musicData->spectralFlux.begin() + max(0, round(startTime * windowRate)), musicData->spectralFlux.begin() + min(musicData->spectralFlux.size() - 1, round(endTime * windowRate)));

	return 1.0 * std::accumulate(sub.begin(), sub.end(), 0.0f) / sub.size();
}

float ZombiETSWave::GetMaximumSpectralFlux()
{
	return maxSpectralFlux;
}

int ZombiETSWave::GetInstantSpectralPeak()
{
	long currentTime = this->Time();
	int sampleRate = musicData->sampleRate;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond
	return musicData->peaks.at(round(currentTime * windowRate));
}

float ZombiETSWave::GetSmoothedSpectralPeak(int interval)
{
	double halfInterval = interval / 2.0;
	long currentTime = this->Time();
	int sampleRate = musicData->sampleRate;
	int samplesPerWindow = musicData->windowSize;
	double windowRate = sampleRate / (double)samplesPerWindow / 1000; // number of values in the spectral flux & peaks per millisecond

	long startTime = currentTime - halfInterval;
	long endTime = currentTime + halfInterval;

	int currentWindow = round(currentTime * windowRate);
	int firstWindow = round(startTime * windowRate);
	int lastWindow = round(endTime * windowRate);

	std::vector<int> sub;

	int windowSize = max(lastWindow - currentWindow, currentWindow - firstWindow);

	if (windowSize > 0)
	{
		for (int i = firstWindow; i < (firstWindow + windowSize); i++)
		{
			if (i >= 0 && i < musicData->peaks.size())
			{
				float hammingCoefficient = 0.54 - 0.46 * cos(2 * M_PI * (i - firstWindow) / (double)windowSize);
				sub.push_back(musicData->peaks.at(i) * hammingCoefficient);
			}
			else
			{
				sub.push_back(0);
			}
		}
	}

	return 1.0 * std::accumulate(sub.begin(), sub.end(), 0.0f);// / sub.size();
}

int ZombiETSWave::GetMaximumSpectralPeak()
{
	return maxSpectralPeak;
}
