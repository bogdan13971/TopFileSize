#pragma once

#include <string>
#include <boost/format.hpp>

constexpr float _1GB = 1 << 30;
constexpr float _1MB = 1 << 20;
constexpr float _1KB = 1 << 10;

std::string getRedableSize(size_t sizeInBytes, bool exact)
{
	static const char* eq = "=";
	static const char* gt = ">";

	boost::format format("%s%6.2f%s");
	
	const char* compSign = eq;
	if (!exact)
	{
		compSign = gt;
	}

	if (sizeInBytes > _1GB)
	{
		return boost::str(format % compSign % (sizeInBytes / _1GB) % " GB");
	}
	else if (sizeInBytes > _1MB)
	{
		return boost::str(format % compSign % (sizeInBytes / _1MB) % " MB");
	}

	return boost::str(format % compSign % (sizeInBytes / _1KB) % " KB");
}