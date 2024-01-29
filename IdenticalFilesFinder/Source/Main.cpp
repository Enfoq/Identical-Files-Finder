#include <iostream>

#include <Public/FileParser.h>
#include <Public/ConfigOptions.h>

int main()
{
	try
	{
		ConfigFile ParserConfig;
		FileParser Parser(&ParserConfig);
		Parser.CollectIdenticalFilesMap();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred during opening " << Config::ConfigFileName << ": " << ex.what() << "\n";
		std::cout << "Available options for a json are next: \n";
		std::cout << Config::TargetFolderPath << "\n";
		std::cout << Config::FileExtensionsToIgnore << " [ ]\n";
		return 1;
	}

	std::cin.get();
	return 0;
}