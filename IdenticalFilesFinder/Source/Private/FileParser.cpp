#include <iostream>

#include <Public/FileParser.h>
#include <Public/FileWrapper.h>


void FileParser::CollectIdenticalFilesMap()
{
	if (Options == nullptr)
	{
		std::cerr << "Config file is a null pointer. Exiting\n";
		return;
	}

	const fs::path Path(Options->GetTargetFolderPath());
	if (!fs::exists(Path))
	{
		std::cerr << "\n[ERROR] Path: " << Path.u8string() << " is not valid.\n";
		return;
	}

	IgnoredExtensions = Options->GetExtensionsToIgnore();

	std::cout << "\n##### Start processing Path: " << Path << "\n";

	CollectFilesWithIdenticalSize(Path);
	ProcessFilesWithIdenticalSize();
	OutputResultOfParsing();

	std::cout << "##### FINISHED #####\n";
}

void FileParser::CollectFilesWithIdenticalSize(const fs::path& Path)
{
	std::vector<fs::path> FoldersToProcess;
	for (const auto& Entry : fs::directory_iterator(Path))
	{
		if (Entry.is_directory() && !IsFolderEmptry(Path))
		{
			FoldersToProcess.push_back(Entry.path());
		}

		if (Entry.is_regular_file() && !IsIgnoredExtension(Entry.path().extension()))
		{
			AddFileToIdenticalSizesFileMap(Entry.path());
		}
	}

	ProcessFoldersConcurrently(FoldersToProcess);
}

void FileParser::AddFileToIdenticalSizesFileMap(const fs::path& FilePath)
{
	FileWrapper File(FilePath);
	auto Size = File.GetFileSize();
	if (Size == 0 || Size == UINTMAX_MAX)
	{
		return;
	}

	std::lock_guard<std::mutex> HashMapLock(IdenticalSizesFileMapMtx);
	IdenticalSizesFileMap[Size].push_back(FilePath.string());
}

void FileParser::ProcessFilesWithIdenticalSize()
{
	for (const auto& MapEntry : IdenticalSizesFileMap)
	{
		if (MapEntry.second.size() < 2)
		{
			continue;
		}

		std::vector<std::thread> WorkingThreads;
		for (const auto& FilePath : MapEntry.second)
		{
			try
			{
				WorkingThreads.emplace_back(&FileParser::AddFileToIdenticalFilesMap, this, FilePath);
			}
			catch (const std::system_error& ex)
			{
				// Handle the thread creation error
				std::cerr << "Error creating thread: " << ex.what() << " (" << ex.code() << ")" << std::endl;
			}
			catch (const std::exception& ex)
			{
				// Handle other exceptions
				std::cerr << "An error occurred: " << ex.what() << std::endl;
			}
		}

		for (auto& Thread : WorkingThreads)
		{
			if (Thread.joinable())
			{
				Thread.join();
			}
		}
	}
}

void FileParser::ProcessFoldersConcurrently(const std::vector<fs::path>& Folders)
{
	std::vector<std::thread> WorkingThreads;
	for (const auto& Folder : Folders)
	{
		try
		{
			WorkingThreads.emplace_back(&FileParser::CollectFilesWithIdenticalSize, this, std::cref(Folder));
		}
		catch (const std::system_error& ex)
		{
			std::cerr << "Error creating thread: " << ex.what() << " (" << ex.code() << ")" << std::endl;
		}
		catch (const std::exception& ex)
		{
			std::cerr << "An error occurred: " << ex.what() << std::endl;
		}
	}

	for (auto& Thread : WorkingThreads)
	{
		if (Thread.joinable())
		{
			Thread.join();
		}
	}
}

void FileParser::AddFileToIdenticalFilesMap(const fs::path& FilePath)
{
	FileWrapper File(FilePath);
	const size_t FileHash = File.GetFileContentHash();

	if (FileHash == 0)
	{
		return;
	}

	std::lock_guard<std::mutex> HashMapLock(IdenticalFilesMapMtx);
	IdenticalFilesMap[FileHash].push_back(FilePath.string());
}

bool FileParser::IsFolderEmptry(const fs::path& FolderPath)
{
	return fs::directory_iterator(FolderPath)._At_end();
}

bool FileParser::IsIgnoredExtension(const fs::path& FileExtensions)
{
	return IgnoredExtensions.find(FileExtensions.string()) != IgnoredExtensions.cend();
}

void FileParser::OutputResultOfParsing()
{
	if (Options == nullptr)
	{
		std::cerr << "Config file is a nullptr. Will print the result to console.\n";
		PrintToConsole();
		
		return;
	}

	WriteToFile();
}

void FileParser::PrintToConsole()
{
	for (const auto& MapEntry : IdenticalFilesMap)
	{
		if (MapEntry.second.size() < 2)
		{
			continue;
		}

		std::cout << "##### Identical Files.\n";
		for (const auto& FilePath : MapEntry.second)
		{
			std::cout << FilePath << "\n";
		}

		std::cout << "#####.\n";
	}
}

void FileParser::WriteToFile()
{
	std::ofstream OutputFile;
	try
	{
		OutputFile.open(Config::OutputFileName);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred: " << ex.what() << "\n";
		std::cerr << "Print to console instead.\n";
		PrintToConsole();

		return;
	}

	for (const auto& MapEntry : IdenticalFilesMap)
	{
		if (MapEntry.second.size() < 2)
		{
			continue;
		}

		OutputFile << "##### Identical Files.\n";
		for (const auto& FilePath : MapEntry.second)
		{
			OutputFile << FilePath << "\n";
		}

		OutputFile << "#####\n";
	}

	std::cout << "Successfully wrote result to " << Config::OutputFileName << "\n";

	OutputFile.close();
}
