#include "pch.h"
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>



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

	fs::recursive_directory_iterator iter(path);
	
	for (auto& p : iter)
		if(fs::is_regular_file(p))
			cout << p << endl;

	return 0;
}
