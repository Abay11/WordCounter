#include "pch.h"
#include <iostream>
#include <string>



int main(int argc, char* argv[])
{
	using namespace std;

	if (argc < 2)
	{
		cout << "Error! Need to set a folder path.";
		return -1;
	}

	string folderPath(argv[1]);

	cout << "Folder path: " << folderPath;

	return 0;
}
