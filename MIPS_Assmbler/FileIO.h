#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileIO
{
private:
	string lastError;
	ifstream fInStream;
public:
	FileIO();
	~FileIO();
	string getLastError() const;
	bool openFile(string file);
	bool closeFile();
	string loadTxtFile();
	string loadTxtFile(string file);
	bool writeTextFile(string fileName, string text);
};
