#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <set>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace Config
{
	static const std::string TargetFolderPath		= "TargetFolderPath";
	static const std::string FileExtensionsToIgnore	= "FileExtensionsToIgnore";
	static const std::string OutputFileName			= "Output.txt";
	static const std::string ConfigFileName			= "Config.json";
}

class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	const std::set<std::string>& GetExtensionsToIgnore() const;
	const fs::path GetTargetFolderPath() const noexcept;

protected:
	void From_Json(const json& Json);
	void InitConfig();
	void PrintOptions() const;

private:
	std::set<std::string> ExtensionsToIgnore;
	fs::path TargetFolderPath;

	std::ifstream FileHandle;
};