#include <iostream>
#include <cstdio>
#include <chrono>
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

void PrettyOutput();

int main()
{
#ifdef _MSC_VER
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
#else
	std::setlocale(LC_ALL, "");
	std::locale::global(std::locale(""));
#endif

	PrettyOutput();

	std::chrono::steady_clock::time_point StartDirtyMeasuring = std::chrono::high_resolution_clock::now();
	try
	{
		ConfigFile ParserConfig;
		FileParser Parser(&ParserConfig);
		Parser.CollectIdenticalFilesMap();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred: " << ex.what() << "\n";
#ifdef _MSC_VER
		system("pause");
#else
		std::cin.get();
#endif
		return 1;
	}

	std::chrono::steady_clock::time_point EndDirtyMeasuring = std::chrono::high_resolution_clock::now();
	auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndDirtyMeasuring - StartDirtyMeasuring);
	std::cout << "[*] Processed in: " << Duration.count() / 1000.f << " seconds.\n";

	
#ifdef _MSC_VER
	system("pause");
#else
	std::cin.get();
#endif

	return 0;
}


void PrettyOutput()
{
	std::cout << "#############################################\n";
	std::cout << "####\tFind Binary identical Files\t#####\n";
	std::cout << "####\tAuthor Dmytro Hurin\t\t#####\n";
	std::cout << "#############################################\n\n";
}
