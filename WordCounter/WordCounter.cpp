#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <future>
#include <chrono>

#include <boost/filesystem.hpp>

static int const THREADS_COUNT = 4;

namespace fs = boost::filesystem;

typedef std::vector<fs::path> FilesVector;

typedef unsigned long long ullong;

FilesVector getFilesList(fs::path& root);

ullong getWordsCount(FilesVector::iterator begin,
	FilesVector::iterator end);

class MyHandler
{
public:
	MyHandler(FilesVector::iterator begin, FilesVector::iterator end)
		: begin(begin)
		, end(end)
	{
		future_ = async(std::launch::async, getWordsCount, begin, end);
	}

	ullong getResult()
	{

		return future_.get();
	}

private:
	FilesVector::iterator begin;
	FilesVector::iterator end;

	std::future<ullong> future_;
};

int main(int argc, char* argv[])
{
	std::string path_arg;
	if (argc < 2)
	{
		std::cout << "Enter folder path: ";
		std::cin >> path_arg;
	}
	else
		path_arg = argv[1];

	fs::path path(path_arg);

	FilesVector files = getFilesList(path);

	namespace chr = std::chrono;
	
	/*
	std::cout << "---WITHOUT THREADING---\n";
	auto simple_start = chr::system_clock::now();
	std::cout << "WORDS COUNT: " << getWordsCount(files.begin(), files.end()) << std::endl;
	auto simple_end = chr::system_clock::now();
	std::cout << "ELAPSED: " << (simple_end - simple_start).count() << std::endl;
	*/

	int files_per_thread = files.size() / THREADS_COUNT;

	unsigned long res = 0;

	//std::vector<std::future<ullong>> futures;

	std::vector<MyHandler> handlers;

	auto threading_start = chr::system_clock::now();

	for (int i = 0; i < THREADS_COUNT; ++i)
	{
		auto start = files.begin() + i * files_per_thread;

		auto end = start;
		if (i + 1 == THREADS_COUNT)
			end = files.end();
		else
			end = files.begin() + (i + 1) * files_per_thread;

		//futures.push_back(async(std::launch::async, getWordsCount, start, end));
		handlers.push_back(MyHandler(start, end));
	}

	for (int i = 0; i < THREADS_COUNT; ++i)
		res += handlers[i].getResult();
		//res += futures[i].get();

	auto threading_end = chr::system_clock::now();

	std::cout << "\n---WITH THREADING---\n";
	std::cout << "WORDS COUNT: " << res << std::endl;
	std::cout << "ELAPSED: " << (threading_end - threading_start).count() << std::endl;

	return 0;
}

FilesVector getFilesList(fs::path& root)
{
	std::vector<fs::path> files;

	fs::recursive_directory_iterator iter(root);

	for (auto& p : iter)
		if (fs::is_regular_file(p) && fs::path(p).extension() == ".txt")
			files.push_back(p);

	return std::move(files);
}

ullong getWordsCount(FilesVector::iterator begin,
	FilesVector::iterator end)
{
	ullong wordsCount = 0;

	for (auto it = begin; it != end; ++it)
	{
		std::ifstream file(it->string());
		
		std::string in;
		while (file >> in)
			++wordsCount;
	}

	return wordsCount;
}
