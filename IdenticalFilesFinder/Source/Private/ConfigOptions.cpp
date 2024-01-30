#include <iostream>
#include <filesystem>
#include <locale>

#include <Public/ConfigOptions.h>

namespace fs = std::filesystem;

ConfigFile::ConfigFile()
{
	InitConfig();
}

ConfigFile::~ConfigFile()
{
	if (FileHandle.is_open())
	{
		FileHandle.close();
	}
}

const std::set<std::string>& ConfigFile::GetExtensionsToIgnore() const
{
	return ExtensionsToIgnore;
}

const fs::path ConfigFile::GetTargetFolderPath() const noexcept
{
	return TargetFolderPath;
}

void ConfigFile::From_Json(const json& Json)
{
	/*
	* Strange move. I know. But with that conversion from std::string to u8path() it properly handles cyrillic symbols in a path.
	*/
	std::string TempStr;
	Json.at(Config::TargetFolderPath).get_to(TempStr);
	TargetFolderPath = fs::u8path(TempStr);
	Json.at(Config::FileExtensionsToIgnore).get_to(ExtensionsToIgnore);
}

void ConfigFile::InitConfig()
{
	try
	{
		FileHandle.open(Config::ConfigFileName);
		FileHandle.imbue(std::locale("en_US.UTF8"));

		json ConfigJson = json::parse(FileHandle);
		FileHandle.close();

		From_Json(ConfigJson);
		PrintOptions();
	}
	catch (const std::exception&)
	{
		throw;
	}
}

void ConfigFile::PrintOptions() const
{
	std::cout << "##### Parse options: \n";
	std::cout << Config::TargetFolderPath << ": " << TargetFolderPath << "\n";
	std::cout << Config::FileExtensionsToIgnore << "[";

	for (const auto& Extension : ExtensionsToIgnore)
	{
		std::cout << Extension << " ";
	}

	std::cout << "]";
	std::cout << "\n#####  -  ######\n";
}
