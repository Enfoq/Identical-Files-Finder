#pragma once
#include <thread>
#include <mutex>
#include <unordered_map>

#include <Public/ConfigOptions.h>

class FileParser
{
public:

	FileParser(const ConfigFile* Opts) : Options(Opts) {};

	/************************************************************************/
	/* This method collects all files concurrently in given path to HashMap <hash, path>.
	* It is synchronized.
	* @return copy of HashMap with hash as a key and path as a value. So don't need to lock on the returned value.
	*/
	/************************************************************************/
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
	std::unordered_map <size_t, std::vector <std::string>> IdenticalFilesMap;

	/**
		Hash map that contains path to all files with identical size (as a key).
	*/
	std::mutex IdenticalSizesFileMapMtx;
	std::unordered_map<size_t, std::vector<std::string>> IdenticalSizesFileMap;

	const ConfigFile *Options = nullptr;
	std::set<std::string> IgnoredExtensions;
};