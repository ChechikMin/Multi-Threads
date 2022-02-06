#pragma once
#include <fstream>
std::fstream file;


class FileWorker
{
public:
	FileWorker() {
		file.open("SomeText.txt");
	};
	~FileWorker() { file.clear(); if (file.is_open()) file.close(); };

private:

};