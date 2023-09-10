#include "FileExplorer.hpp"
#include "DirectoryTree.hpp"
#include "FileSizeHelper.hpp"

FileExplorer::FileExplorer(
	const std::string& root, 
	const NameFunctor& nameFunctor,
	const ValueFunctor& valueFunctor)
	: directoryTree{buildDirectoryTree(root, nameFunctor)},
	console{},
	currentNode{directoryTree.begin()},
	valueFunctor{valueFunctor}
{}

std::unique_ptr<ui::EventListener> FileExplorer::createEventListener()
{
	auto eventListener = std::make_unique<ui::EventListener>();
	eventListener->registerEventHandler(ui::EVENT_TYPE::UP, [&]() {
		if (lut::prevSibling(currentNode) != directoryTree.end())
		{
			currentNode = lut::prevSibling(currentNode);
		}
	});

	eventListener->registerEventHandler(ui::EVENT_TYPE::DOWN, [&]() {
		if (lut::nextSibling(currentNode) != directoryTree.end())
		{
			currentNode = lut::nextSibling(currentNode);
		}
	});

	eventListener->registerEventHandler(ui::EVENT_TYPE::ENTER, [&]() {
		if (lut::firstChild(currentNode) != directoryTree.end())
		{
			currentNode = lut::firstChild(currentNode);
		}
	});

	eventListener->registerEventHandler(ui::EVENT_TYPE::BACK, [&]() {
		if (lut::parent(currentNode) != directoryTree.begin())
		{
			currentNode = lut::parent(currentNode);
		}
	});

	eventListener->registerKeyPressEventHandler('c', [&]() {
		console.clearLog();
	});

	eventListener->registerKeyPressEventHandler('f', [&]() {
		if (!currentNode->isLoaded())
		{
			directoryTree.update(currentNode, valueFunctor);

			//update this item and all children
			auto item = console.getMenu().getSelectedItem();
			item->setDirty();
			
			//update all parents
			auto parents = console.getMenu().getParents();
			for (auto parent : parents)
			{
				parent->setBaseDirty();
			}

			footer->setDirty();
		}
	});

	eventListener->registerKeyPressEventHandler('g', [&]() {
		auto parent = lut::parent(currentNode);
		directoryTree.update(parent, valueFunctor);

		//update parent
		auto sub = console.getMenu().getParentSubMenu();
		sub->setDirty();

		//update all parents
		auto parents = console.getMenu().getParents();
		for (auto parent : parents)
		{
			parent->setBaseDirty();
		}

		footer->setDirty();
	});

	return eventListener;
}

void FileExplorer::init()
{
	auto menu = createMenu();
	auto eventListener = createEventListener();

	console.setMenu(std::move(menu));
	console.setEventListener(std::move(eventListener));

	console.setUpdateCallback([&]() {
		std::string line = currentNode->getKey();
		console.log(std::move(line));
	});
}

void FileExplorer::start()
{
	console.start();
}

std::string updateLabel(lut::NodeIterator it)
{
	std::string line =
		getRedableSize(it->getValue(), it->isLoaded());
	line.append(" ").append(it->getKey());

	return line;
}

std::unique_ptr<ui::Menu>  FileExplorer::createMenu()
{
	auto menu = std::make_unique<ui::Menu>();
	auto it = lut::firstChild(directoryTree.begin());

	currentNode = it;

	while (it != directoryTree.end())
	{
		if (lut::firstChild(it) == directoryTree.end())
		{
			auto item = menu->addItem(it->getKey());
			item->setUpdateCallback([&, it](){
				return updateLabel(it);
			});
		}
		else
		{
			auto sub = menu->addSubmenu(it->getKey());
			sub->setUpdateCallback([&, it]() {
				return updateLabel(it);
			});
			createDirRecursive(sub, lut::firstChild(it));
		}

		it = lut::nextSibling(it);
	}

	menu->addHeader("File Size Explorer");
	footer = menu->addFooter("TOTAL ");
	footer->setUpdateCallback([root = directoryTree.begin()]() {
		auto it = root;
		std::string line("TOTAL: ");
		line.append(getRedableSize(it->getValue(), it->isLoaded()));

		return line;
	});

	return menu;
}

void FileExplorer::createDirRecursive(ui::SubMenu* menu, lut::NodeIterator it)
{
	while (it != directoryTree.end())
	{
		if (lut::firstChild(it) == directoryTree.end())
		{
			auto item = menu->addItem(it->getKey());
			item->setUpdateCallback([&, it]() {
				return updateLabel(it);
			});
		}
		else
		{
			auto sub = menu->addSubMenu(it->getKey());
			sub->setUpdateCallback([&, it]() {
				return updateLabel(it);
			});
			createDirRecursive(sub, lut::firstChild(it));
		}

		it = lut::nextSibling(it);
	}
}