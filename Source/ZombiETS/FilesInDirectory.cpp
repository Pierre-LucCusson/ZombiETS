// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "FilesInDirectory.h"
#pragma comment(lib, "shell32.lib")

bool FilesInDirectory::FilesEndsWith(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

FilesInDirectory::FilesInDirectory(std::string endingBy)
{
	filesCount = 0;
	fileEndingBy = endingBy;
}

FilesInDirectory::~FilesInDirectory()
{
}

std::vector<std::string> FilesInDirectory::GetFileNames()
{
	return filesList;
}

std::string FilesInDirectory::GetFileName(int i)
{
	return filesList.at(i);
}

int FilesInDirectory::GetFilesCount()
{
	return filesCount;
}

/*
	Get a random file return empty string if not found
*/
std::string FilesInDirectory::GetRandomFile()
{
	if (this->GetFilesCount() == 0)
		return "";
	srand(time(NULL));
	return this->GetFileName(rand() % this->GetFilesCount());
}

void FilesInDirectory::ReadDirectory(const std::wstring & name)
{
	std::wstring pattern = std::wstring(name);
	pattern.append(TEXT("\\"));
	std::wstring patternP = pattern + TEXT("*");
	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(patternP.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			std::wstring ws = pattern + std::wstring(data.cFileName);
			std::string s;
			s.assign(ws.begin(), ws.end());
			if (data.nFileSizeLow > 0 && FilesEndsWith(s, fileEndingBy)) {
				filesCount++;
				filesList.push_back(s);
			}
			else if (ws != pattern + TEXT(".") && ws != pattern + TEXT("..")) {
				ReadDirectory(ws);
			}
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}

std::wstring FilesInDirectory::GetMyDocuments()
{
	WCHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	return std::wstring(my_documents);
}

std::wstring FilesInDirectory::GetExeDirectory()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}
