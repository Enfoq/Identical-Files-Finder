#include <iostream>
#include <filesystem>

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
	Json.at(Config::TargetFolderPath).get_to(TargetFolderPath);
	Json.at(Config::FileExtensionsToIgnore).get_to(ExtensionsToIgnore);
}

void ConfigFile::InitConfig()
{
	try
	{
		FileHandle.open(Config::ConfigFileName);
		json ConfigJson = json::parse(FileHandle);

		From_Json(ConfigJson);
		PrintOptions();

		FileHandle.close();
	}
	catch (const std::exception&)
	{
		throw;
	}
}

void ConfigFile::PrintOptions() const
{
	std::cout << "##### Parse options: \n";
	std::cout << " TargetFolderPath: " << TargetFolderPath << "\n";
	std::cout << " ExtensionsToIgnore: [";
	for (const auto& Extension : ExtensionsToIgnore)
	{
		std::cout << Extension << " ";
	}
	std::cout << "]";

	std::cout << "\n#####  -  ######\n";
}
