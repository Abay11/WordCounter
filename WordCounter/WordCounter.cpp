#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <future>
#include <chrono>

#include "Worker.h"

static int const THREADS_COUNT = 4;

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

	namespace chr = std::chrono;

	std::cout << "---WITHOUT THREADING---\n";
	
	Worker::initFilesList(path_arg);

	auto simple_start = chr::system_clock::now();

	Worker::getWordsCount();

	std::cout << "Total words count: " << Worker::totalWords_  << std::endl;

	auto simple_end = chr::system_clock::now();

	std::cout << "ELAPSED: " << (simple_end - simple_start).count() << std::endl;






	std::cout << "\n---WITH THREADING---\n";

	Worker::totalWords_ = 0;

	Worker::initFilesList(path_arg);

	auto threading_start = chr::system_clock::now();
	
	std::vector<std::future<void>> workers;
	for (int i = 0; i < THREADS_COUNT; ++i)
	{
		workers.push_back(std::async(std::launch::async, Worker::getWordsCount));
	}
	
	for (int i = 0; i < THREADS_COUNT; ++i)
	{
		workers[i].wait();
	}

	auto threading_end = chr::system_clock::now();

	std::cout << "Total words count: " << Worker::totalWords_ << std::endl;

	std::cout << "ELAPSED: " << (threading_end - threading_start).count() << std::endl;

	return 0;
}