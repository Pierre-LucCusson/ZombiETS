#include "stdafx.h"
#include <iostream>
#include "FilesDirectory.h"
#include "Windows.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <shlobj.h>
#include <locale>
#include <stdlib.h>

#pragma comment(lib, "shell32.lib")


bool FilesDirectory::FilesEndsWith(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

FilesDirectory::FilesDirectory(std::string endingBy)
{
	fileEndingBy = endingBy;
}


FilesDirectory::~FilesDirectory()
{
}

std::string FilesDirectory::GetFileName(int i) {
	return filesList.at(i);
}

std::vector<std::string> FilesDirectory::GetFileNames()
{
	return filesList;
}

void FilesDirectory::ReadDirectory(const std::wstring& name) {
	FilesDirectory::ReadDirectory(name, filesList, fileEndingBy);
}

std::string FilesDirectory::GetRandomFile() {
	int size = this->GetFileNames().size();
	if (size == 0)
		return NULL;
	srand(time(NULL));
	return this->GetFileName(rand() % size);

}

void FilesDirectory::ReadDirectory(const std::wstring& name, std::vector<std::string>& v, std::string fileType)
{
	std::wstring pattern = std::wstring(name);
	pattern.append(TEXT("\\"));
	std::wstring patternP = pattern + TEXT("*");
	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(patternP.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			std::wstring ws = pattern+std::wstring(data.cFileName);
			std::string s;
			s.assign(ws.begin(),ws.end());
			if (data.nFileSizeLow > 0 && FilesEndsWith(s, fileType)) {
				v.push_back(s);
			}
			else if(ws != pattern+TEXT(".") && ws != pattern+TEXT("..")){
				ReadDirectory(ws, v);
			}
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}

std::wstring FilesDirectory::GetMyDocuments() {
	WCHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	return std::wstring(my_documents);
}
