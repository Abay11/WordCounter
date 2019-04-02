#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

static int const THREADS_COUNT = 4;

using namespace std;

namespace fs = boost::filesystem;

std::vector<fs::path> getFilesList(fs::path& root)
{
	std::vector<fs::path> files;

	fs::recursive_directory_iterator iter(root);

	for (auto& p : iter)
		if (fs::is_regular_file(p))
			files.push_back(p);

	return std::move(files);
}

long getWordsCount(std::vector<fs::path>::iterator begin,
	std::vector<fs::path>::iterator end)
{
	long wordsCount = 0;

	for (auto it = begin; it != end; ++it)
	{
		ifstream file(it->string());
		
		string in;
		while (file >> in)
			++wordsCount;
		
	}

	return wordsCount;
}

int main(int argc, char* argv[])
{
	using namespace std;
	
	namespace fs = boost::filesystem;

	if (argc < 2)
	{
		cout << "Error! Need to set a folder path.";
		return -1;
	}

	fs::path path(argv[1]);

	vector<fs::path> files = getFilesList(path);

	int files_per_thread = files.size() / THREADS_COUNT;

	long res = 0;

	for (int i = 0; i < THREADS_COUNT; ++i)
	{
		auto start = files.begin() + i * files_per_thread;

		auto end = start;
		if (i + 1 == THREADS_COUNT)
			end = files.end();
		else
			end = files.begin() + (i + 1) * files_per_thread;

		
		res += getWordsCount(start, end);
	}
	
	cout << "WORDS COUNT (without division): " << getWordsCount(files.begin(), files.end()) << endl;

	cout << "WORDS COUNT (with division): " << res << endl;



	//for (auto& p : files)
	//	cout << p << endl;

	return 0;
}
