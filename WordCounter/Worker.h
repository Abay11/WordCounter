#pragma once

#include <mutex>
#include <vector>

#include <boost/filesystem.hpp>


namespace ns
{
	namespace fs = boost::filesystem;

	typedef std::vector<fs::path> FilesVector;

	typedef unsigned long long ullong;
}

class Worker
{

public:
	Worker()
	{
	}

	static void initFilesList(const std::string& root)
	{
		ns::fs::recursive_directory_iterator iter(root);

		for (auto& p : iter)
			if (ns::fs::is_regular_file(p) && ns::fs::path(p).extension() == ".txt")
			{
				files_.push_back(p);
			}
	}

	static void getWordsCount()
	{
		ns::ullong currentFileWords = 0;

		ns::fs::path file;
		while (!(file = pop_file()).empty())
		{
			std::ifstream source(file.string());

			std::string in;
			while (source.good())
			{
				source >> in;

				++currentFileWords;
			}

		}

		std::lock_guard<std::mutex> lg(mutex_);

		totalWords_ += currentFileWords;
	}

	static ns::fs::path pop_file()
	{
		std::lock_guard<std::mutex> lg(mutex_);

		if (files_.empty())
			return ns::fs::path();
		else
		{
			auto path = files_.back();
			files_.pop_back();
			return path;
		}
	}

	static ns::ullong totalWords_;

private:
	static std::mutex mutex_;

	static ns::FilesVector files_;
};

//assume they are in .cpp
std::mutex Worker::mutex_;

ns::ullong Worker::totalWords_ = 0;

ns::FilesVector Worker::files_;