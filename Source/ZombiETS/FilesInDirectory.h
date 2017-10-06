// Fill out your copyright notice in the Description page of Project Settings.
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <shlobj.h>
#include <locale>
#include <stdlib.h>
#pragma once

/**
 * 
 */
class ZOMBIETS_API FilesInDirectory
{
private:
	std::vector<std::string> filesList;
	int filesCount;
	std::string fileEndingBy;

	static bool FilesEndsWith(std::string const & value, std::string const & ending);
public:
public:
	FilesInDirectory(std::string endingBy = "");
	~FilesInDirectory();

	std::vector<std::string> GetFileNames();
	std::string GetFileName(int i);
	int GetFilesCount();
	std::string GetRandomFile();

	void ReadDirectory(const std::wstring& name);

	static std::wstring GetMyDocuments();
};
