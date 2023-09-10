#pragma once

#include <LUT/Tree.hpp>
#include <functional>
#include <boost/filesystem.hpp>

lut::Tree buildDirectoryTree(
	const std::string&, 
	const std::function<std::string(const boost::filesystem::path&)>&);