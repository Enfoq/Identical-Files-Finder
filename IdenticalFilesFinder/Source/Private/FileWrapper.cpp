#include <Public/FileWrapper.h>

// Hash specialization for std::vector<std::byte>
namespace std {
	template <>
	struct hash<std::vector<std::byte>> {
		size_t operator()(const std::vector<std::byte>& Vec) const {
			// A simple hash combining the hashes of individual bytes
			size_t Result = 0;
			for (const auto& byte : Vec) {
				Result ^= std::hash<std::byte>{}(byte)+0x9e3779b9 + (Result << 6) + (Result >> 2);
			}
			return Result;
		}
	};
}

FileWrapper::~FileWrapper()
{
	if (File.is_open())
	{
		File.close();
	}
}

const size_t FileWrapper::GetFileContentHash()
{
	auto FileSize = GetFileSize();
	if (FileSize == 0 || FileSize == UINTMAX_MAX)
	{
		std::cerr << "Cannot retrieve size of file: " << "\n";

		return 0;
	}

	File.open(Path, std::ios_base::binary);
	if (!File.is_open())
	{
		std::cerr << "Error opening file: " << Path << "\n";

		return 0;
	}

	try
	{
		std::vector<std::byte> Buffer(FileSize);
		File.read(reinterpret_cast<char*>(Buffer.data()), FileSize);

		return std::hash<std::vector<std::byte>>{}(Buffer);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Cannot read file: " << Path << "\n";
		std::cerr << "Exception: " << e.what() << "\n";

		return 0;
	}
	catch (...)
	{
		std::cerr << "Unknown exception happened with file: " << Path.string() << "\n";

		return 0;
	}
}

const std::uintmax_t FileWrapper::GetFileSize()
{
	return fs::file_size(Path);
}
