#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

class FileWrapper
{
public:
	FileWrapper(const fs::path& FilePath) : Path(FilePath) {};
	~FileWrapper();

	const size_t GetFileContentHash();

	const std::uintmax_t GetFileSize();

private:
	fs::path Path{};
	std::ifstream File;
};
