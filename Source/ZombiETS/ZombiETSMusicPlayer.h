// Fill out your copyright notice in the Description page of Project Settings.
#include "Windows.h"
#include "MMSystem.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>

#pragma once

/**
 * 
 */
class ZOMBIETS_API ZombiETSMusicPlayer
{
public:
	ZombiETSMusicPlayer();
	~ZombiETSMusicPlayer();

	void Play(FString filename);
	void Play(std::string filename);
	void Play(std::wstring filename);
	void Stop();
};
