#include <iostream>

#include <Public/FileParser.h>
#include <Public/ConfigOptions.h>

int main()
{
	std::cout << "##### - PARSER - #####\n";
	std::cout << "\nAvailable options for a json are next: \n";
	std::cout << Config::TargetFolderPath << "\n";
	std::cout << Config::FileExtensionsToIgnore << " [ ]\n\n";

	try
	{
		ConfigFile ParserConfig;
		FileParser Parser(&ParserConfig);
		Parser.CollectIdenticalFilesMap();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred: " << ex.what() << "\n";
		
		return 1;
	}

	std::cin.get();
	return 0;
}