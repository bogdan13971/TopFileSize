#pragma once

#include <ConsoleUI/ConsoleUI.hpp>
#include <LUT/Tree.hpp>
#include <LUT/NodeIterator.hpp>
#include <boost/filesystem.hpp>

class FileExplorer
{
public:
	using NameFunctor = std::function<std::string(const boost::filesystem::path&)>;
	using ValueFunctor = lut::Tree::ValueFunctor;
private:
	lut::Tree directoryTree;
	ui::ConsoleUI console;

	lut::NodeIterator currentNode;
	ValueFunctor valueFunctor;

	ui::Item* footer;

private:
	std::unique_ptr<ui::Menu> createMenu();
	std::unique_ptr<ui::EventListener> createEventListener();

	void createDirRecursive(ui::SubMenu*, lut::NodeIterator);
public:
	FileExplorer(const std::string&,const NameFunctor&, const ValueFunctor&);
	void init();
	void start();
};