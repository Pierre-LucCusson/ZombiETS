#include <sys/types.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#pragma once

class FilesDirectory
{

private:
	std::vector<std::string> filesList;

	std::string fileEndingBy;

	static bool FilesEndsWith(std::string const & value, std::string const & ending);
public:
	FilesDirectory(std::string endingBy="");
	~FilesDirectory();

	std::vector<std::string> GetFileNames();
	std::string GetFileName(int i);
	std::string GetRandomFile();

	void ReadDirectory(const std::wstring& name);
	static void ReadDirectory(const std::wstring& name, std::vector<std::string>& v, std::string fileType="");
	
	static std::wstring GetMyDocuments();
};

