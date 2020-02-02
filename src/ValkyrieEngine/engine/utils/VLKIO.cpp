#include "VLKIO.h"

#include <fstream>

using namespace vlk;

StringCollection IO::ReadFile(const std::string& path)
{
	std::ifstream stream;

	stream.open(path);

	if (!stream.is_open())
	{
		LogError("IO", "Failed to open file {" + path + "}");
		return StringCollection();
	}

	std::string line;
	std::vector<std::string> lines;

	while (std::getline(stream, line))
	{
		lines.push_back(line + "\n");
	}

	stream.close();

	return StringCollection(lines);
}