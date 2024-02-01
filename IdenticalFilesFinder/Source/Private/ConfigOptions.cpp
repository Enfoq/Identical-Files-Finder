#include <iostream>
#include <filesystem>
#include <locale>
#include <codecvt>

#include <Public/ConfigOptions.h>

namespace fs = std::filesystem;

ConfigFile::ConfigFile()
{
	HelpInfoPrint();
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

void ConfigFile::HelpInfoPrint() const
{
	json Json;
	Json[Config::TargetFolderPath] = "<path>";
	Json[Config::FileExtensionsToIgnore] = { "extensions" };

	std::cout << "[*] Config file options available:\n";
	std::cout << Json.dump(2);
	std::cout << "\n\n";
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
		json ConfigJson = json::parse(FileHandle);
		FileHandle.close();

		From_Json(ConfigJson);
		PrintOptions(ConfigJson);
	}
	catch (const std::exception&)
	{
		throw;
	}
}

void ConfigFile::PrintOptions(const json& Json) const
{
	std::cout << "[*]Enabled options: \n";
	std::string DumpJson = Json.dump(2);
	std::cout << fs::u8path(DumpJson).string() << "\n";
}
