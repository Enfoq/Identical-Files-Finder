#include <iostream>
#include <cstdio>
#ifdef _MSC_VER
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#else
#include <clocale>
#include <locale>
#endif

#include <Public/FileParser.h>
#include <Public/ConfigOptions.h>

int main()
{
#ifdef _MSC_VER
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
#else
	std::setlocale(LC_ALL, "");
	std::locale::global(std::locale(""));
#endif

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
