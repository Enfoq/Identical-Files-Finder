#pragma once
#include <thread>
#include <mutex>
#include <unordered_map>

#include <Public/ConfigOptions.h>

class FileParser
{
public:

	FileParser(const ConfigFile* Opts) : Options(Opts) {};
	void CollectIdenticalFilesMap();

private:
	void CollectFilesWithIdenticalSize(const fs::path& Path);
	void AddFileToIdenticalSizesFileMap(const fs::path& FilePath);
	void ProcessFilesWithIdenticalSize();
	void ProcessFoldersConcurrently(const std::vector<fs::path>& Folders);
	void AddFileToIdenticalFilesMap(const fs::path& FilePath);
	bool IsFolderEmptry(const fs::path& FolderPath);
	bool IsIgnoredExtension(const fs::path& FileExtensions);
	void OutputResultOfParsing();
	void PrintToConsole();
	void WriteToFile();

private:
	/**
		Hash map that contains path to all files with identical hash (as a key).
	*/
	std::mutex IdenticalFilesMapMtx;
	std::unordered_map <size_t, std::vector <fs::path>> IdenticalFilesMap;

	/**
		Hash map that contains path to all files with identical size (as a key).
	*/
	std::mutex IdenticalSizesFileMapMtx;
	std::unordered_map<size_t, std::vector<fs::path>> IdenticalSizesFileMap;

	const ConfigFile *Options = nullptr;
	std::set<std::string> IgnoredExtensions;
};
