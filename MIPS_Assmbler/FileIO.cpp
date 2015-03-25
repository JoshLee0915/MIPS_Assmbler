#include "FileIO.h"

FileIO::FileIO()
{
}
FileIO::~FileIO()
{
	// close the file stream if open
	closeFile();
}
string FileIO::getLastError() const
{
	return lastError;
}
bool FileIO::openFile(string file)
{
	//Check if the file is empty
	if(file.empty())
	{
		lastError = "[ERR] No file name was given";
		return false;
	}

	try
	{
		fInStream.clear();
		fInStream.open(file.c_str());

		if(fInStream==0)
		{
			lastError = "[ERR] File failed to open";
			return false; // The file failed to open
		}
	}
	// Catch stream exceptions while opening
	catch(ifstream::failure e)
	{
		lastError = "[ERR] ";
		lastError += e.what();
		return false;
	}
	// to be safe catch all other exceptions
	catch(...)
	{
		lastError = "[ERR] Unknown exception occured";
		return false;
	}
	return true;
}
bool FileIO::closeFile()
{
	// check if the stream is open and if it is close it
	if(fInStream.is_open())
		fInStream.close();

	else
	{
		lastError = "[ERR] Unable to close file";
		return false;
	}

	return true;
}
string FileIO::loadTxtFile()
{
	string line = "";
	string loadedFile = "";

	try
	{
		// check if there is a stream open
		if(fInStream==0)
		{
			lastError = "[ERR] No file is open";
			return "";
		}
		// clear all error flags
		fInStream.clear();
		// set to the begining of the file
		fInStream.seekg(0);

		// read the file
		while(getline(fInStream, line))
		{
			loadedFile += line;
			loadedFile += "\n";
		}
		
	}
	// Catch stream exceptions 
	catch(ifstream::failure e)
	{
		lastError = "[ERR] ";
		lastError += e.what();
		return "";
	}
	// to be safe catch all other exceptions
	catch(...)
	{
		lastError = "[ERR] Unknown exception occured";
		return "";
	}
	return loadedFile;
}
string FileIO::loadTxtFile(string file)
{
	// open the file
	if(!openFile(file))
		return "";

	// read the file and return the text
	return loadTxtFile();
}
bool FileIO::writeTextFile(std::string fileName, std::string text)
{
	//Check if the fileName is empty
	if(fileName.empty())
	{
		lastError = "[ERR] No file name was given";
		return false;
	}

	try
	{
		// open the stream
		ofstream fout (fileName.c_str());

		if(!fout.is_open())
		{
			lastError = "[ERR] Unable to open file";
			return false;
		}

		// write the text
		fout << text;

		// close the stream
		fout.close();
	}
	// catch write exceptions
	catch(ofstream::failure e)
	{
		lastError = "[ERR] ";
		lastError += e.what();
		return false;
	}
	// to be safe catch all other exceptions
	catch(...)
	{
		lastError = "[ERR] Unknown exception occured";
		return false;
	}
	return true;
}